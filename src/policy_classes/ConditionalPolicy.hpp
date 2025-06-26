// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ConditionalPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CONDITIONAL_POLICY_HPP
#define INCLUDED_CONDITIONAL_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ConditionPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    template <typename ConditionalData, srcDispatch::ParserState DispatchEvent>
    class ConditionalPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    protected:
        ConditionalData data;

        std::unique_ptr<ConditionPolicy> conditionPolicy;
        std::unique_ptr<BlockPolicy>     blockPolicy;

    public:
        ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeConditionalPolicyHandlers();
        }

        ~ConditionalPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ConditionalData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if (typeid(ConditionPolicy) == typeid(*policy)) {
                data.condition = DeltaElement(ctx.diffStack.back().operation, policy->Data<ConditionData>());
            } else if (typeid(BlockPolicy) == typeid(*policy)) {
                data.block = DeltaElement(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

        void InitializeConditionalPolicyHandlers() {

            using namespace srcDispatch;

            openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if (!depth) {
                    depth = ctx.depth;
                    data = ConditionalData{};
                    data.startLineNumber = ctx.startLineNumber;
                    data.endLineNumber = ctx.endLineNumber;
                    CollectConditionHandlers();
                    CollectBlockHandlers();
                }
            };

            // end of policy
            closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if (!depth || depth != ctx.depth) return ;

                depth = 0;
                NotifyAll(ctx);
                InitializeConditionalPolicyHandlers();
            };
        }

        void CollectConditionHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
                if(!depth) return; 

                if (!conditionPolicy) {
                    conditionPolicy = make_unique_policy<ConditionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(conditionPolicy.get());
            };
        }

        void CollectBlockHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
                if(!depth) return; 

                if (!blockPolicy) {
                    blockPolicy = make_unique_policy<BlockPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(blockPolicy.get());
            };
        }
    };

}

#endif
