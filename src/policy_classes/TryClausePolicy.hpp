// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TryClausePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_TRY_CLAUSE_POLICY_HPP
#define INCLUDED_TRY_CLAUSE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <BlockPolicy.hpp>

#include <string>
#include <iostream>

namespace srcDispatch {

    template <typename ClauseParam, srcDispatch::ParserState DispatchEvent>
    class TryClausePolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    protected:
        ClauseParam   data;

        std::unique_ptr<BlockPolicy> blockPolicy;

    public:
        TryClausePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeHandlers();
        }

        ~TryClausePolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ClauseParam>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} // doesn't use other parsers

        void InitializeHandlers() {
            using namespace srcDispatch;

            openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = ClauseParam{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;

                CollectHandlers();
            };

            // end of policy
            closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeHandlers();
            };
        }

        virtual void CollectHandlers() {
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
