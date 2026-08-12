// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TryPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_TRY_POLICY_HPP
#define INCLUDED_TRY_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <BlockPolicy.hpp>
#include <CatchPolicy.hpp>
#include <FinallyPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct TryData : public ElementData {

        DeltaElement<std::shared_ptr<BlockData>> block;
        std::vector<DeltaElement<std::any>>      clauses;
    };

    class TryPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        TryData data;

        std::unique_ptr<BlockPolicy>   blockPolicy;
        std::unique_ptr<CatchPolicy>   catchPolicy;
        std::unique_ptr<FinallyPolicy> finallyPolicy;

    public:
        TryPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeTryPolicyHandlers();
        }

        ~TryPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<TryData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else if(typeid(CatchPolicy) == typeid(*policy)) {
                data.clauses.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<CatchData>()));
            } else if(typeid(FinallyPolicy) == typeid(*policy)) {
                data.clauses.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<FinallyData>()));
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} // doesn't use other parsers

    private:
        void InitializeTryPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::trystmt] = [this](srcSAXEventContext &ctx) {
                if(depth) return; 

                depth = ctx.depth;
                data = TryData{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;
                CollectBlockHandlers();
                CollectClauseHandlers();
            };

            // end of policy
            closeEventMap[ParserState::trystmt] = [this](srcSAXEventContext &ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeTryPolicyHandlers();
            };
        }

        void CollectBlockHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::block] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!blockPolicy) {
                    blockPolicy = make_unique_policy<BlockPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(blockPolicy.get());
            };
        }

        void CollectClauseHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::catchstmt] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!catchPolicy) {
                    catchPolicy = make_unique_policy<CatchPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(catchPolicy.get());
            };

            openEventMap[ParserState::finallystmt] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!finallyPolicy) {
                    finallyPolicy = make_unique_policy<FinallyPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(finallyPolicy.get());
            };
        }

    };

}

#endif
