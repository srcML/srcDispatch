// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DeclPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_DECL_POLICY_HPP
#define INCLUDED_DECL_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <Access.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <GenericPolicy.hpp>
#include <TypePolicy.hpp>
#include <NamePolicy.hpp>
#include <ExpressionPolicy.hpp>

#include <string>
#include <vector>

namespace srcDispatch {

    struct DeclData : public ElementData {
        
        std::vector<DeltaElement<std::shared_ptr<GenericData>>>    generics;
        DeltaElement<std::shared_ptr<Access>>                      accessSpecifier;
        DeltaElement<std::shared_ptr<TypeData>>                    type;
        DeltaElement<std::shared_ptr<NameData>>                    name;
        DeltaElement<std::shared_ptr<ExpressionData>>              init;
        std::vector<DeltaElement<std::shared_ptr<ExpressionData>>> arguments;
        DeltaElement<std::shared_ptr<ExpressionData>>              range;

        DeltaElement<bool> isStatic;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {

            std::string str;
            if(type) {
                str += type.ToString(operation);
            }

            if(name) {
                if(name.IsOfOperation(operation)) {
                    str += ' ';
                }
                str += name.ToString(operation);
            }

            if(init) {
                if(init.IsOfOperation(operation)) {
                    str +=  " = ";
                }
                str += init.ToString(operation);
            }

            if(!arguments.empty()) {

                bool isArgument = false;
                for(const DeltaElement<std::shared_ptr<ExpressionData>>& argument : arguments) {

                    bool outputRaw = argument.IsOfOperation(operation);
                    if(outputRaw) {
                        if(!isArgument) {
                            str += '(';
                        } else {
                            str += ", ";
                        }
                        isArgument = true;
                    }
                    str += argument.ToString(operation);
                }

                if(isArgument) {
                    str += ')';
                }                
            }

            if(range) {
                if(range.IsOfOperation(operation)) {
                    str +=  " : ";
                }
               str += range.ToString(operation);
            }

            return str;           
        }
    };

    class DeclPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        DeclData data;

        std::unique_ptr<GenericPolicy>    genericPolicy;
        std::unique_ptr<TypePolicy>       typePolicy;
        std::unique_ptr<NamePolicy>       namePolicy;
        std::unique_ptr<ExpressionPolicy> exprPolicy;

    public:
        DeclPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeDeclPolicyHandlers();
        }

        ~DeclPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<DeclData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            using namespace srcDispatch;
            if(typeid(GenericPolicy) == typeid(*policy)) {
                data.generics.emplace_back(ctx.diffStack.back().operation, policy->Data<GenericData>());
            } else if(typeid(TypePolicy) == typeid(*policy)) {
                data.type = DeltaElement(ctx.diffStack.back().operation, policy->Data<TypeData>());
            } else if(typeid(NamePolicy) == typeid(*policy)) {
                data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
            } else if(typeid(ExpressionPolicy) == typeid(*policy)) {
                if(ctx.IsOpen(ParserState::range)) {
                    data.range.Update(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                } else if(ctx.IsOpen(ParserState::init)) {
                    data.init.Update(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                } else if(ctx.IsOpen(ParserState::argumentlist)) {
                    data.arguments.emplace_back(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                } else {
                    throw std::string("Unhandled ExpressionPolicy condition");
                }
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeDeclPolicyHandlers() {
            using namespace srcDispatch;

            // start of policy
            std::function<void(srcSAXEventContext& ctx)> startDecl = [this](srcSAXEventContext &ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = DeclData{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;

                CollectGenericHandlers();
                CollectSpecifiersHandlers();
                CollectTypeHandlers();
                CollectNameHandlers();
                CollectInitHandlers();
                CollectArgumentList();
                CollectRangeHandlers();
            };

            openEventMap[ParserState::decl]      = startDecl;
            // For generic template parameter
            openEventMap[ParserState::parameter] = startDecl;

            // close policy
            std::function<void(srcSAXEventContext& ctx)> endDecl = [this](srcSAXEventContext &ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                data = DeclData{};
                InitializeDeclPolicyHandlers();
            };

            closeEventMap[ParserState::decl]      = endDecl;
            closeEventMap[ParserState::parameter] = endDecl;

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

        void CollectSpecifiersHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::specifier] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext &ctx) {
                    if(ctx.currentToken == "static") {
                        data.isStatic.Update(ctx.diffStack.back().operation, true);
                    }
                };
            };

            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                NopCloseEvents({ParserState::tokenstring});
            };
        }

        void CollectTypeHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::type] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!typePolicy) {
                    typePolicy = make_unique_policy<TypePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(typePolicy.get());
            };
        }

        void CollectNameHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        }

        void CollectInitHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::init] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                openEventMap[ParserState::expr] = [this](srcSAXEventContext &ctx) {
                    if(!exprPolicy) {
                        exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
                };
            };

            closeEventMap[ParserState::init] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::expr});
            };

        }

        void CollectArgumentList() {
            using namespace srcDispatch;
            openEventMap[ParserState::argumentlist] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                openEventMap[ParserState::argument] = [this](srcSAXEventContext &ctx) {
                    openEventMap[ParserState::expr] = [this](srcSAXEventContext &ctx) {
                        if(!exprPolicy) {fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
                            exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                        }
                        ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
                    };
                };

                closeEventMap[ParserState::argument] = [this](srcSAXEventContext &ctx) {
                    NopOpenEvents({ParserState::expr});
                };

            };

            closeEventMap[ParserState::argumentlist] = [this](srcSAXEventContext &ctx) {
                NopOpenEvents({ParserState::argument});
            };
        }

        void CollectRangeHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::range] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                openEventMap[ParserState::expr] = [this](srcSAXEventContext &ctx) {
                    if(!exprPolicy) {
                        exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
                };
            };

            closeEventMap[ParserState::range] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                NopOpenEvents({ParserState::expr});
            };
        }
    };
}

#endif
