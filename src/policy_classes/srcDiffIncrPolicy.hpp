// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffIncrPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_INCR_POLICY_HPP
#define INCLUDED_SRCDIFF_INCR_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <srcDiffExpressionPolicy.hpp>
#include <srcDiffBlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDiffDispatch {

    struct IncrData {

        unsigned int startLineNumber;
        std::vector<DeltaElement<std::shared_ptr<ExpressionData>>> exprs;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            if(exprs.empty()) return "";

            std::string str;
            bool printComma = false;
            for(const DeltaElement<std::shared_ptr<ExpressionData>>& expr : exprs) {

                bool outputRaw = expr.IsOfOperation(operation);
                if(outputRaw) {
                    if(printComma) {
                        str += ", ";
                    }
                    printComma = true;
                }

                str += expr.ToString(operation);
            }

            return str;
        }
    };

    class IncrPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        IncrData data;

        std::unique_ptr<ExpressionPolicy> exprPolicy;

    public:
        IncrPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeIncrPolicyHandlers();
        }

        ~IncrPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<IncrData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(ExpressionPolicy) == typeid(*policy)) {
                data.exprs.emplace_back(ctx.diffStack.back().operation, policy->Data<ExpressionData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeIncrPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
                if(!depth) {
                    depth = ctx.depth;
                    data = IncrData{};
                    data.startLineNumber = ctx.startLineNumber;
                    CollectExpressionHandlers();
                }
            };

            // end of policy
            closeEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeIncrPolicyHandlers();
            };
        }

        void CollectExpressionHandlers() {
            using namespace srcDispatch;
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
