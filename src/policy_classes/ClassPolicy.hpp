// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ClassPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CLASS_POLICY_HPP
#define INCLUDED_CLASS_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <Access.hpp>
#include <GenericPolicy.hpp>
#include <NamePolicy.hpp>
#include <DeclStmtPolicy.hpp>
#include <FunctionPolicy.hpp>
#include <TypeDefPolicy.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

    struct ParentData;

    struct ClassData : public ElementData {
        enum ClassType : std::size_t { CLASS, STRUCT };

        std::vector<std::string> namespaces;

        std::string language;
        std::string filename;

        /*** @todo implement */
        std::set<std::string> stereotypes;

        std::vector<DeltaElement<std::shared_ptr<GenericData>>>  generics;
        DeltaElement<ClassType>                                  type;

        DeltaElement<std::shared_ptr<Access>>                   accessSpecifier;
        std::vector<DeltaElement<std::shared_ptr<std::string>>> specifiers;

        DeltaElement<std::shared_ptr<NameData>>                  name;
        std::vector<DeltaElement<std::shared_ptr<ParentData>>>   parents;

        std::vector<DeltaElement<std::shared_ptr<DeclStmtData>>> fields;
        std::vector<DeltaElement<std::shared_ptr<FunctionData>>> constructors;
        DeltaElement<std::shared_ptr<FunctionData>>              destructor;

        std::vector<DeltaElement<std::shared_ptr<FunctionData>>> operators;
        std::vector<DeltaElement<std::shared_ptr<FunctionData>>> methods;

        std::vector<DeltaElement<std::shared_ptr<TypeDefData>>>  typedefs;
        std::vector<DeltaElement<std::shared_ptr<ClassData>>>    innerClasses;

        DeltaElement<bool> isAbstract;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            std::string str;

            if(name && name.IsOfOperation(operation)) {
                str = type == CLASS? "class " : "struct "; 
            }

            str += name.ToString(operation);

            return str;
        }
    };

    struct ParentData {
        DeltaElement<std::shared_ptr<NameData>>        name;
        DeltaElement<bool>                             isVirtual;
        DeltaElement<std::shared_ptr<Access>> accessSpecifier;
    };

    class ClassPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ClassData data;

        DeltaElement<std::shared_ptr<Access>> currentRegion;

        std::unique_ptr<GenericPolicy>  genericPolicy;
        std::unique_ptr<NamePolicy>     namePolicy;
        std::unique_ptr<DeclStmtPolicy> declStmtPolicy;
        std::unique_ptr<FunctionPolicy> functionPolicy;
        std::unique_ptr<TypeDefPolicy>  typeDefPolicy;
        std::unique_ptr<ClassPolicy>    classPolicy;

        static const std::unordered_map<srcDispatch::ParserState, ClassData::ClassType> stateToTypeMapper;

    public:
        ClassPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{}, currentRegion() {
            InitializeClassPolicyHandlers();
        }

        ~ClassPolicy() {}

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(GenericPolicy) == typeid(*policy)) {
                data.generics.emplace_back(ctx.diffStack.back().operation, policy->Data<GenericData>());
            } else if(typeid(NamePolicy) == typeid(*policy)) {
                if(ctx.And({srcDispatch::ParserState::super})) {
                    data.parents.back()->name.Update(ctx.diffStack.back().operation, policy->Data<NameData>());
                } else {
                    data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
                }
            } else if(typeid(DeclStmtPolicy) == typeid(*policy)) {
                data.fields.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclStmtData>());
                if(currentRegion) {
                    for (DeltaElement<std::shared_ptr<DeclData>>& decl : data.fields.back()->decls) {
                        decl->accessSpecifier = currentRegion;
                    }
                }
            } else if(typeid(FunctionPolicy) == typeid(*policy)) {

                std::shared_ptr<FunctionData> f_data = policy->Data<FunctionData>();
                FunctionData::FunctionType    f_type = f_data->type.GetElement();
                if(currentRegion) {
                   f_data->accessSpecifier = currentRegion;
                }
                if(f_data->isPureVirtual) {
                    data.isAbstract.Update(ctx.diffStack.back().operation, true);
                }

                if(f_type == FunctionData::CONSTRUCTOR) {
                    data.constructors.emplace_back(ctx.diffStack.back().operation, f_data);
                } else if(f_type == FunctionData::DESTRUCTOR) {
                    data.destructor.Update(ctx.diffStack.back().operation, f_data);
                } else if(f_type == FunctionData::OPERATOR) {
                    data.operators.emplace_back(ctx.diffStack.back().operation, f_data);
                } else {
                    data.methods.emplace_back(ctx.diffStack.back().operation, f_data);
                }
            } else if(typeid(TypeDefPolicy) == typeid(*policy)) {
                data.typedefs.emplace_back(ctx.diffStack.back().operation, policy->Data<TypeDefData>());
            } else if(typeid(ClassPolicy) == typeid(*policy)) {
                srcDispatch::DiffOperation operation = ctx.diffStack.back().isConvert? srcDispatch::COMMON : ctx.diffStack.back().operation;
                data.innerClasses.emplace_back(operation, policy->Data<ClassData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    protected:
        std::any DataInner() const override { return std::make_shared<ClassData>(data); }

    private:
        void InitializeClassPolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            std::function<void(srcDispatch::srcSAXEventContext& )> startPolicy = [this](srcSAXEventContext& ctx) {
                if(!depth) {

                    depth = ctx.depth;
                    data = ClassData{};
                    data.namespaces = ctx.currentNamespaces;
                    data.startPosition = ctx.startPosition;
                    data.endPosition   = ctx.endPosition;
                    data.accessSpecifier = NULL_ACCESS;
                    std::map<std::string, std::string>::const_iterator stereotype_attr_itr = ctx.attributes.find("stereotype");
                    if(stereotype_attr_itr != ctx.attributes.end()) {
                        std::istringstream stereostring(stereotype_attr_itr->second);
                        data.stereotypes = std::set<std::string>(std::istream_iterator<std::string>(stereostring), std::istream_iterator<std::string>());
                    }

                    data.type = DeltaElement(ctx.diffStack.back().operation, stateToTypeMapper.at(ctx.dispatcher->CurrentPState()));
                    data.name = DeltaElement<std::shared_ptr<NameData>>();
                    data.language = ctx.currentFileLanguage;
                    data.filename = ctx.currentFilePath;
                    CollectGenericHandlers();
                    CollectNameHandlers();
                    CollectSuperHanders();
                    CollectBlockHanders();
                    CollectSpecifierHandlers();
                } else {

                    if(ctx.diffStack.back().isConvert && ctx.diffStack.back().operation == srcDispatch::INSERT) {
                        data.type.Update(ctx.diffStack.back().operation, stateToTypeMapper.at(ctx.dispatcher->CurrentPState()));
                    } else {

                        if(!classPolicy) {
                            classPolicy = make_unique_policy<ClassPolicy>({this});
                        }
                        ctx.dispatcher->AddListenerDispatch(classPolicy.get());
                    }
                }
            };

            // end of policy
            std::function<void(srcDispatch::srcSAXEventContext&)> endPolicy = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeClassPolicyHandlers();
            };
            openEventMap[ParserState::classn] = startPolicy;
            closeEventMap[ParserState::classn] = endPolicy;

            openEventMap[ParserState::structn] = startPolicy;
            closeEventMap[ParserState::structn] = endPolicy;
        }

        void CollectGenericHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::templates] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!genericPolicy) {
                    genericPolicy = make_unique_policy<GenericPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(genericPolicy.get());
            };
        }

        void CollectNameHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::name});
                NopCloseEvents({ParserState::name});
            };
        }

        void CollectSuperHanders() {
            using namespace srcDispatch;
            openEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::specifier});
                NopCloseEvents({ParserState::specifier});

                openEventMap[ParserState::super] = [this](srcSAXEventContext& ctx) {
                    data.parents.emplace_back(ctx.diffStack.back().operation, 
                        std::make_shared<ParentData>(
                            ParentData{DeltaElement<std::shared_ptr<NameData>>(), DeltaElement<bool>(), DeltaElement<std::shared_ptr<Access>>()}
                                                    )
                    );

                    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
                        if(!namePolicy) {
                            namePolicy = make_unique_policy<NamePolicy>({this});
                        }
                        ctx.dispatcher->AddListenerDispatch(namePolicy.get());
                    };

                    closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                        if(ctx.And({ParserState::specifier})) {
                            std::shared_ptr<Access> specifier = AccessFactory(ctx.currentToken);
                            if(specifier == NULL_ACCESS) {
                                data.parents.back()->isVirtual.Update(ctx.diffStack.back().operation, true);
                            } else {
                                data.parents.back()->accessSpecifier.Update(ctx.diffStack.back().operation, specifier);
                            }
                        }
                    };
                };
                closeEventMap[ParserState::super] = [this](srcSAXEventContext& ctx) {
                    NopCloseEvents({ParserState::tokenstring});
                };
            };

            closeEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::super});
            };
        }

        void CollectBlockHanders() {
            using namespace srcDispatch;
            openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::name, ParserState::specifier, ParserState::super_list, ParserState::super});
                NopCloseEvents({ParserState::name, ParserState::specifier, ParserState::super_list, ParserState::tokenstring});

                openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
                    if(!declStmtPolicy) {
                        declStmtPolicy = make_unique_policy<DeclStmtPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(declStmtPolicy.get());
                };

                std::function<void(srcSAXEventContext&  ctx)> functionEvent = [this](srcSAXEventContext& ctx) {
                    if(!functionPolicy) {
                        functionPolicy = make_unique_policy<FunctionPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(functionPolicy.get());
                };

                openEventMap[ParserState::function]        = functionEvent;
                openEventMap[ParserState::functiondecl]    = functionEvent;
                openEventMap[ParserState::constructor]     = functionEvent;
                openEventMap[ParserState::constructordecl] = functionEvent;
                openEventMap[ParserState::destructor]      = functionEvent;
                openEventMap[ParserState::destructordecl]  = functionEvent;

                openEventMap[ParserState::typedefdecl] = [this](srcSAXEventContext& ctx) {
                    if(!typeDefPolicy) {
                        typeDefPolicy = make_unique_policy<TypeDefPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(typeDefPolicy.get());
                };
            };

            openEventMap[ParserState::publicaccess] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(ctx.diffStack.back().isConvert) {
                    currentRegion.Update(ctx.diffStack.back().operation, PUBLIC_ACCESS);
                } else {
                    currentRegion = DeltaElement(ctx.diffStack.back().operation, PUBLIC_ACCESS);
                }
            };

            openEventMap[ParserState::protectedaccess] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(ctx.diffStack.back().isConvert) {
                    currentRegion.Update(ctx.diffStack.back().operation, PROTECTED_ACCESS);
                } else {
                    currentRegion = DeltaElement(ctx.diffStack.back().operation, PROTECTED_ACCESS);
                }
            };

            openEventMap[ParserState::privateaccess] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(ctx.diffStack.back().isConvert) {
                    currentRegion.Update(ctx.diffStack.back().operation, PRIVATE_ACCESS);
                } else {
                    currentRegion = DeltaElement(ctx.diffStack.back().operation, PRIVATE_ACCESS);
                }
            };

            closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::block, ParserState::function, ParserState::functiondecl,
                               ParserState::constructor, ParserState::constructordecl, ParserState::destructor, ParserState::destructordecl,
                               ParserState::declstmt,
                               ParserState::publicaccess, ParserState::protectedaccess, ParserState::privateaccess});
                NopCloseEvents({ParserState::block});
            };
        }

        void CollectSpecifierHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                    std::shared_ptr<Access> specifier = AccessFactory(ctx.currentToken);
                    if(specifier == NULL_ACCESS) {
                        data.specifiers.emplace_back(ctx.diffStack.back().operation, std::make_shared<std::string>(ctx.currentToken));
                    } else {
                        data.accessSpecifier.Update(ctx.diffStack.back().operation, specifier);
                    }
                };
            };
                    
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;
                NopCloseEvents({ParserState::tokenstring});
            };
        }
    };
}

#endif
