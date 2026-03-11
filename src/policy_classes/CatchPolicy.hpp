// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file CatchPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CATCH_POLICY_HPP
#define INCLUDED_CATCH_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <DeclPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct CatchData : public ElementData {

        std::vector<DeltaElement<std::shared_ptr<DeclData>>> parameters;
        DeltaElement<std::shared_ptr<BlockData>> block;
    };

    class CatchPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        CatchData   data;

        std::unique_ptr<DeclPolicy>  declPolicy;
        std::unique_ptr<BlockPolicy> blockPolicy;

    public:
        CatchPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeCatchPolicyHandlers();
        }

        ~CatchPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<CatchData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(DeclPolicy) == typeid(*policy)) {
                data.parameters.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} // doesn't use other parsers

    private:
        void InitializeCatchPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::catchstmt] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = CatchData{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;
                CollectParametersHandlers();
                CollectBlockHandlers();
            };

            // end of policy
            closeEventMap[ParserState::catchstmt] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeCatchPolicyHandlers();
            };
        }

        void CollectParametersHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx [[maybe_unused]]) {
                if(!depth) return;

                openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
                    if(!declPolicy) {
                        declPolicy = make_unique_policy<DeclPolicy>({this});
                    }
                    ctx.dispatcher->AddListenerDispatch(declPolicy.get());
                };
            };

            closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx [[maybe_unused]]) {
                if(!depth) return;

                NopOpenEvents({ParserState::parameter});
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
