// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ForLike.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_FORLIKE_POLICY_HPP
#define INCLUDED_FORLIKE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <ControlPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class BlockPolicy;
    struct BlockData;

    template <typename ForLikeParam, srcDispatch::ParserState DispatchEvent>
    class ForLike :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ForLikeParam data;

        std::unique_ptr<ControlPolicy> controlPolicy;
        std::unique_ptr<BlockPolicy>   blockPolicy;

    public:
        ForLike(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeForLikeHandlers();
        }

        ~ForLike() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ForLikeParam>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(ControlPolicy) == typeid(*policy)) {
                data.control = DeltaElement(ctx.diffStack.back().operation, policy->Data<ControlData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} // doesn't use other parsers

    private:
        void InitializeForLikeHandlers() {
            using namespace srcDispatch;

            openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = ForLikeParam{};
                data.startPosition = ctx.startPosition;
                data.endPosition = ctx.endPosition;
                CollectControlHandlers();
                CollectBlockHandlers();
            };

            // end of policy
            closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeForLikeHandlers();
            };
        }

        void CollectControlHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!controlPolicy) {
                    controlPolicy = make_unique_policy<ControlPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(controlPolicy.get());
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
