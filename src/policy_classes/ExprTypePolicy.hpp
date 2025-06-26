// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffExprTypePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_EXPR_TYPE_POLICY_HPP
#define INCLUDED_SRCDIFF_EXPR_TYPE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ExpressionPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDiffDispatch {

    template <typename ExprTypeData, srcDispatch::ParserState DispatchEvent>
    class ExprTypePolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ExprTypeData data;

        std::unique_ptr<ExpressionPolicy> exprPolicy;

    public:
        ExprTypePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeExprTypePolicyHandlers();
        }

        ~ExprTypePolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ExprTypeData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(ExpressionPolicy) == typeid(*policy)) {
                data.expr.Update(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeExprTypePolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!depth) {
                    depth = ctx.depth;
                    data = ExprTypeData{};
                    CollectExpressionHandlers();
                }
            };

            // end of policy
            closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeExprTypePolicyHandlers();
            };
        }

        void CollectExpressionHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!exprPolicy)
                    exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        }
    };

}

#endif
