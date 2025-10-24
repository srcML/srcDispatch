// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ForPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_FOR_POLICY_HPP
#define INCLUDED_FOR_POLICY_HPP

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

    struct ForData : public ElementData {

        DeltaElement<std::shared_ptr<ControlData>> control;
        DeltaElement<std::shared_ptr<BlockData>>   block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return control.ToString(operation);
        }
    };

    class ForPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ForData data;

        std::unique_ptr<ControlPolicy> controlPolicy;
        std::unique_ptr<BlockPolicy>   blockPolicy;

    public:
        ForPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeForPolicyHandlers();
        }

        ~ForPolicy() {}

    protected:
        std::any DataInner() const { return std::make_shared<ForData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
            if(typeid(ControlPolicy) == typeid(*policy)) {
                data.control = DeltaElement(ctx.diffStack.back().operation, policy->Data<ControlData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block = DeltaElement(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {} // doesn't use other parsers

    private:
        void InitializeForPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = ForData{};
                data.startPosition = ctx.startPosition;
                data.endPosition = ctx.endPosition;
                CollectControlHandlers();
                CollectBlockHandlers();
            };

            // end of policy
            closeEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeForPolicyHandlers();
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
