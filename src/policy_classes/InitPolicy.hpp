// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file InitPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_INIT_POLICY_HPP
#define INCLUDED_SRCDIFF_INIT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ExpressionPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct InitData {

        unsigned int startLineNumber;
        std::vector<DeltaElement<std::any>> inits;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            if(inits.empty()) return "";

            std::string str;
            bool printComma = false;
            for (const DeltaElement<std::any>& init : inits) {

                bool outputRaw = init.IsOfOperation(operation);
                if(outputRaw) {
                    if(printComma) {
                        str += ", ";
                    }
                    printComma = true;
                }

                if(init.GetElement().type() == typeid(std::shared_ptr<DeclData>)) {
                    str += init.ToString<std::shared_ptr<DeclData>>(operation);
                } else {
                    str += init.ToString<std::shared_ptr<ExpressionData>>(operation);
                }
            }

            return str;
        }
    };

    class InitPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        InitData data;

        std::unique_ptr<DeclPolicy>       declPolicy;
        std::unique_ptr<ExpressionPolicy> exprPolicy;

    public:
        InitPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeInitPolicyHandlers();
        }

        ~InitPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<InitData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(ExpressionPolicy) == typeid(*policy)) {
                if(data.inits.size() && ctx.diffStack.back().isReplace && data.inits.back().GetElement().type() == typeid(std::shared_ptr<ExpressionData>)) {
                    data.inits.back().Update(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                } else {
                    data.inits.emplace_back(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                }
            } else if(typeid(DeclPolicy) == typeid(*policy)) {
                // not sure how safe GetElement is here
                srcDispatch::DiffOperation operation = ctx.diffStack.back().operation;
                std::shared_ptr<DeclData> decl = policy->Data<DeclData>();
                if(data.inits.size() && data.inits.back().GetElement().type() == typeid(std::shared_ptr<DeclData>)) {
                    DeltaElement<std::shared_ptr<TypeData>> typeData(operation, std::any_cast<std::shared_ptr<DeclData>>(data.inits.back().GetElement())->type.GetElement()->copyAs(operation));
                    decl->type     = typeData;
                    decl->isStatic = std::any_cast<std::shared_ptr<DeclData>>(data.inits.back().GetElement())->isStatic;
                }
                data.inits.emplace_back(operation, decl);
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeInitPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
                if(!depth) {
                    depth = ctx.depth;
                    data = InitData{};
                    data.startLineNumber = ctx.startLineNumber;
                    CollectExpressionHandlers();
                }
            };

            // end of policy
            closeEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeInitPolicyHandlers();
            };
        }

        void CollectExpressionHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!declPolicy) {
                    declPolicy = make_unique_policy<DeclPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(declPolicy.get());
            };

            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!exprPolicy) {
                    exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        }
    };

}

#endif
