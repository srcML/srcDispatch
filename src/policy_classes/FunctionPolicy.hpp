// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file FunctionPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_FUNCTION_POLICY_HPP
#define INCLUDED_SRCDIFF_FUNCTION_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <AccessSpecifier.hpp>
#include <DeltaElement.hpp>

#include <GenericPolicy.hpp>
#include <NamePolicy.hpp>
#include <TypePolicy.hpp>
#include <CallPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

    struct FunctionData {

        enum FunctionType { CONSTRUCTOR, DESTRUCTOR, OPERATOR, FUNCTION };

        /*** @todo fix */
        // std::vector<DeltaElement<std::string>> namespaces;
        std::vector<std::string> namespaces;

        unsigned int lineNumber;
        std::string language;
        std::string filename;

        std::vector<DeltaElement<std::shared_ptr<GenericData>>>  generics;
        DeltaElement<FunctionType>    type;
        DeltaElement<AccessSpecifier> accessSpecifier;

        std::vector<DeltaElement<std::shared_ptr<std::string>>> leadingSpecifiers;
        std::vector<DeltaElement<std::shared_ptr<std::string>>> trailingSpecifiers;

        DeltaElement<bool> isDecl;
        DeltaElement<bool> isPureVirtual;
        DeltaElement<bool> isDelete;

        /*** @todo implement */
        std::set<std::string> stereotypes;

        DeltaElement<std::shared_ptr<TypeData>> returnType;
        DeltaElement<std::shared_ptr<NameData>> name;
        std::vector<DeltaElement<std::shared_ptr<DeclData>>> parameters;
        std::vector<DeltaElement<std::shared_ptr<CallData>>> memberInitList;
        DeltaElement<std::shared_ptr<BlockData>> block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {

            std::string signature;

            for(DeltaElement<std::shared_ptr<std::string>> specifier : leadingSpecifiers) {
                if(specifier.IsOfOperation(operation)) {
                    signature += *specifier.GetOfOperation(operation) + ' ';
                }
            }

            signature += returnType.ToString(operation);

            if(returnType.IsOfOperation(operation)) {
                signature += ' ';
            }

            signature += name.ToString(operation);

            if(name.IsOfOperation(operation)) {
                signature += '(';
            }

            bool outputComma = false;
            for (const DeltaElement<std::shared_ptr<DeclData>>& parameter : parameters) {
                bool outputRaw = parameter.IsOfOperation(operation);
                if(outputRaw) {
                    if(outputComma) {
                        signature += ", ";
                    }
                    outputComma = true;
                }
                signature += parameter.ToString(operation);
            }

            if(name.IsOfOperation(operation)) {
                signature += ')';
            }

            for(DeltaElement<std::shared_ptr<std::string>> specifier : trailingSpecifiers) {
                if(specifier.IsOfOperation(operation)) {
                    signature += ' ' + *specifier.GetOfOperation(operation);
                }
            }


            if(isPureVirtual.IsOfOperation(operation) && isPureVirtual.GetOfOperation(operation)) {
                signature += " = 0";
            }

            if(isDelete.IsOfOperation(operation) && isDelete.GetOfOperation(operation)) {
                signature += " = delete";
            }

            if(isDecl.IsOfOperation(operation)) {
                if(isDecl.GetOfOperation(operation)) {
                   signature += ';';
                } else {
                    signature += " {}";
                }
            }

            return signature;
        }
    };

    class FunctionPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        FunctionData data;

        bool beforeParameters;

        std::unique_ptr<GenericPolicy> genericPolicy;
        std::unique_ptr<TypePolicy>    typePolicy;
        std::unique_ptr<NamePolicy>    namePolicy;
        std::unique_ptr<DeclPolicy>    declPolicy;
        std::unique_ptr<CallPolicy>    callPolicy;
        std::unique_ptr<BlockPolicy>   blockPolicy;

    public:
        FunctionPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{}, beforeParameters(true) {
            InitializeFunctionPolicyHandlers();
        }

        ~FunctionPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<FunctionData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(GenericPolicy) == typeid(*policy)) {
                data.generics.emplace_back(ctx.diffStack.back().operation, policy->Data<GenericData>());
            } else if(typeid(TypePolicy) == typeid(*policy)) {
                data.returnType = DeltaElement(ctx.diffStack.back().operation, policy->Data<TypeData>());
            } else if(typeid(NamePolicy) == typeid(*policy)) {
                data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
            } else if(typeid(DeclPolicy) == typeid(*policy)) {
                data.parameters.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclData>());
            } else if(typeid(CallPolicy) == typeid(*policy)) {
                data.memberInitList.emplace_back(ctx.diffStack.back().operation, policy->Data<CallData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeFunctionPolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            std::function<void(srcSAXEventContext& ctx)> startFunction = [this](srcSAXEventContext& ctx) {
                if(!depth) {
                    depth = ctx.depth;
                    data = FunctionData{};
                    data.namespaces = ctx.currentNamespaces;
                    data.lineNumber = ctx.startLineNumber;
                    data.language = ctx.currentFileLanguage;
                    data.filename = ctx.currentFilePath;
                    std::map<std::string, std::string>::const_iterator stereotype_attr_itr = ctx.attributes.find("stereotype");
                    if(stereotype_attr_itr != ctx.attributes.end()) {
                        std::istringstream stereostring(stereotype_attr_itr->second);
                        data.stereotypes = std::set<std::string>(std::istream_iterator<std::string>(stereostring), std::istream_iterator<std::string>());
                    }
                    if(ctx.currentTag == "function" || ctx.currentTag == "function_decl") {
                        if(ctx.isOperator) {
                            data.type.Update(ctx.diffStack.back().operation, FunctionData::OPERATOR);
                        } else {
                            data.type.Update(ctx.diffStack.back().operation, FunctionData::FUNCTION);
                        }
                    } else if(ctx.currentTag == "constructor" || ctx.currentTag == "constructor_decl") {
                        data.type.Update(ctx.diffStack.back().operation, FunctionData::CONSTRUCTOR);
                    } else if(ctx.currentTag == "destructor" || ctx.currentTag == "destructor_decl") {
                        data.type.Update(ctx.diffStack.back().operation, FunctionData::DESTRUCTOR);
                    }

                    data.isDecl.Update(ctx.diffStack.back().operation, ctx.currentTag == "function_decl" || ctx.currentTag == "constructor_decl" || ctx.currentTag == "destructor_decl");

                    CollectXMLAttributeHandlers();
                    CollectGenericHandlers();
                    CollectTypeHandlers();
                    CollectNameHandlers();
                    CollectParameterHandlers();
                    CollectCallHandlers();
                    CollectOtherHandlers();
                    CollectBlockHandlers();
                }
            };

            // end of policy
            std::function<void(srcSAXEventContext& ctx)> endFunction = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeFunctionPolicyHandlers();
            };

            openEventMap[ParserState::function]        = startFunction;
            openEventMap[ParserState::functiondecl]    = startFunction;
            openEventMap[ParserState::constructor]     = startFunction;
            openEventMap[ParserState::constructordecl] = startFunction;
            openEventMap[ParserState::destructor]      = startFunction;
            openEventMap[ParserState::destructordecl]  = startFunction;

            closeEventMap[ParserState::function]        = endFunction;
            closeEventMap[ParserState::functiondecl]    = endFunction;
            closeEventMap[ParserState::constructor]     = endFunction;
            closeEventMap[ParserState::constructordecl] = endFunction;
            closeEventMap[ParserState::destructor]      = endFunction;
            closeEventMap[ParserState::destructordecl]  = endFunction;
        }

        void CollectXMLAttributeHandlers() {}

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

        void CollectTypeHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!typePolicy) {
                    typePolicy = make_unique_policy<TypePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(typePolicy.get());
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
        }

        void CollectParameterHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
                    if(!declPolicy) {
                        declPolicy = make_unique_policy<DeclPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(declPolicy.get());
                };
            };

            closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                beforeParameters = false;
                NopOpenEvents({ParserState::parameter});
            };
        }

        void CollectCallHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!callPolicy) {
                    callPolicy = make_unique_policy<CallPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(callPolicy.get());
            };
        }

        void CollectOtherHandlers() {
            using namespace srcDispatch;
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(ctx.And({ParserState::specifier})) {
                    if(ctx.currentToken == "delete") {
                        data.isDelete.Update(ctx.diffStack.back().operation, true);
                    } else if(beforeParameters) {
                        data.leadingSpecifiers.emplace_back(ctx.diffStack.back().operation,  std::make_shared<std::string>(ctx.currentToken));
                    } else {
                        data.trailingSpecifiers.emplace_back(ctx.diffStack.back().operation, std::make_shared<std::string>(ctx.currentToken));
                    }
                } else if(ctx.And({ParserState::literal})) {
                    data.isPureVirtual.Update(ctx.diffStack.back().operation, true);
                }
            };
        }

        void CollectBlockHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!blockPolicy) {
                    blockPolicy = make_unique_policy<BlockPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(blockPolicy.get());
            };
        }
    };

}

#endif
