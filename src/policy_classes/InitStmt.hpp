// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file InitStmt.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_INITSTMT_POLICY_HPP
#define INCLUDED_INITSTMT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <BlockStmt.hpp>
#include <InitPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class BlockPolicy;
    struct BlockData;

    template <typename InitStmtParam, srcDispatch::ParserState DispatchEvent>
    class InitStmt : public BlockStmt<InitStmtParam, DispatchEvent> {

    private:
        InitStmtParam data;

        std::unique_ptr<InitPolicy> initPolicy;

        using EventListener::openEventMap;
        using EventListener::closeEventMap;

    public:
        InitStmt(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : BlockStmt<InitStmtParam, DispatchEvent>(listeners), data{} {
            InitializeInitStmtHandlers();
        }

        ~InitStmt() {}

    protected:
        std::any DataInner() const override { return std::make_shared<InitStmtParam>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(InitPolicy) == typeid(*policy)) {
                data.init = DeltaElement(ctx.diffStack.back().operation, policy->Data<InitData>());
            } else if(typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} // doesn't use other parsers

    private:
        void InitializeInitStmtHandlers() {
            using namespace srcDispatch;
            openEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(this->depth) return;

                this->depth = ctx.depth;
                data = InitStmtParam{};
                data.startPosition = ctx.startPosition;
                data.endPosition = ctx.endPosition;
                CollectInitHandlers();
                this->CollectBlockHandlers();
            };

            // end of policy
            closeEventMap[DispatchEvent] = [this](srcSAXEventContext& ctx) {
                if(!this->depth || this->depth != ctx.depth) return;

                this->depth = 0;
                this->NotifyAll(ctx);
                InitializeInitStmtHandlers();
            };
        }

        void CollectInitHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
                if(!this->depth) return;

                if(!initPolicy) {
                    initPolicy = make_unique_policy<InitPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(initPolicy.get());
            };
        }
    };

}

#endif
