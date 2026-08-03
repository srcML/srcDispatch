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

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <ConditionPolicy.hpp>
#include <BlockStmtPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ConditionalData : public ElementData {

        DeltaElement<std::shared_ptr<ConditionData>> condition;
        DeltaElement<std::shared_ptr<BlockData>>     block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return condition.ToString(operation);
        }
    };

    template <typename ConditionalDataParam, srcDispatch::ParserState DispatchEvent>
    class ConditionalPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    protected:
        ConditionalDataParam data;

        std::unique_ptr<ConditionPolicy> conditionPolicy;
        std::unique_ptr<BlockPolicy>     blockPolicy;

    public:
        ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            BlockStmt<InitStmtParam, DispatchEvent>::InitializeHandlers();
        }

        ~ConditionalPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ConditionalDataParam>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(ConditionPolicy) == typeid(*policy)) {
                data.condition = DeltaElement(ctx.diffStack.back().operation, policy->Data<ConditionData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

        void CollectHandlers() override {
            using namespace srcDispatch;
            
            BlockStmt<ConditionalDataParam, DispatchEvent>::CollectHandlers();

            openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
                if(!depth) return; 

                if(!conditionPolicy) {
                    conditionPolicy = make_unique_policy<ConditionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(conditionPolicy.get());
            };
        }
    };

}

#endif
