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

#include <BlockStmt.hpp>
#include <ControlPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class BlockPolicy;
    struct BlockData;

    template <typename ForLikeParam, srcDispatch::ParserState DispatchEvent>
    class ForLike : public BlockStmt<ForLikeParam, DispatchEvent> {

    private:
        ForLikeParam data;

        std::unique_ptr<ControlPolicy> controlPolicy;

    public:
        ForLike(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : BlockStmt<ForLikeParam, DispatchEvent>(listeners), data{} {
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
            this->openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(this->depth) return;

                this->depth = ctx.depth;
                data = ForLikeParam{};
                data.startPosition = ctx.startPosition;
                data.endPosition = ctx.endPosition;
                CollectControlHandlers();
            };

            // end of policy
            this->closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!this->depth || this->depth != ctx.depth) return;

                this->depth = 0;
                this->NotifyAll(ctx);
                InitializeForLikeHandlers();
            };
        }

        void CollectControlHandlers() {
            using namespace srcDispatch;
            this->openEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
                if(!this->depth) return;

                if(!controlPolicy) {
                    controlPolicy = make_unique_policy<ControlPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(controlPolicy.get());
            };
        }
    };

}

#endif
