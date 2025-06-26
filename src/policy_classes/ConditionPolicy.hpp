// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ConditionPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www..org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_CONDITION_POLICY_HPP
#define INCLUDED_SRCDIFF_CONDITION_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ExpressionPolicy.hpp>
#include <DeclPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ConditionData {
        std::vector<DeltaElement<std::any>> conditions;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            if(conditions.empty()) return "";

            std::string str;
            bool printComma = false;
            for (const DeltaElement<std::any>& condition : conditions) {

                bool outputRaw = condition.IsOfOperation(operation);
                if(outputRaw) {
                    if (printComma) {
                        str += ", ";
                    }
                    printComma = true;
                }

                if (condition.GetElement().type() == typeid(std::shared_ptr<DeclData>)) {
                    str += condition.ToString<std::shared_ptr<DeclData>>(operation);
                } else {
                    str += condition.ToString<std::shared_ptr<ExpressionData>>(operation);
                }
            }

            return str;
        }
    };

    // Collect the expression in the return
    //
    class ConditionPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ConditionData data;

        std::unique_ptr<ExpressionPolicy> exprPolicy;
        std::unique_ptr<DeclPolicy>       declPolicy;

    public:
        ConditionPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeConditionPolicyHandlers();
        }

        ~ConditionPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ConditionData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if (typeid(ExpressionPolicy) == typeid(*policy)) {
                if(data.conditions.size() && ctx.diffStack.back().isReplace && data.conditions.back().GetElement().type() == typeid(std::shared_ptr<ExpressionData>)) {
                    data.conditions.back().Update(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                } else {
                    data.conditions.emplace_back(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
                }
            } else if (typeid(DeclPolicy) == typeid(*policy)) {
                // not sure how safe GetElement is here
                std::shared_ptr<DeclData> decl = policy->Data<DeclData>();
                if (data.conditions.size() && decl->type->types.empty()
                 && data.conditions.back().GetElement().type() == typeid(std::shared_ptr<DeclData>)) {
                    decl->type     = std::any_cast<std::shared_ptr<DeclData>>(data.conditions.back().GetElement())->type;
                    decl->isStatic = std::any_cast<std::shared_ptr<DeclData>>(data.conditions.back().GetElement())->isStatic;
                }
                srcDispatch::DiffOperation operation = ctx.diffStack.back().operation;
                data.conditions.emplace_back(operation, decl);
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeConditionPolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
                if (!depth) {
                    depth = ctx.depth;
                    data = ConditionData{};
                    CollectExpressionHandlers();
                    CollectDeclPolicyHandlers();
                }
            };

            // end of policy
            closeEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
                if (!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeConditionPolicyHandlers();
            };
        }

        void CollectExpressionHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if (!depth) return;

                if (!exprPolicy) {
                    exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        }

        void CollectDeclPolicyHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
                if (!depth) return;

                if (!declPolicy) {
                    declPolicy = make_unique_policy<DeclPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(declPolicy.get());
            };
        }
    };

}

#endif
