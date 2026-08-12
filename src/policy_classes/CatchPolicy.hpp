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

#include <TryClausePolicy.hpp>
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

    class CatchPolicy : public TryClausePolicy<CatchData, srcDispatch::ParserState::catchstmt> {

    private:
        std::unique_ptr<DeclPolicy>  declPolicy;

    public:
        CatchPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : TryClausePolicy<CatchData, srcDispatch::ParserState::catchstmt>(listeners) {}

        ~CatchPolicy() {}

    protected:
        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(DeclPolicy) == typeid(*policy)) {
                data.parameters.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclData>());
            } else if (typeid(BlockPolicy) == typeid(*policy)) {
                data.block.Update(ctx.diffStack.back().operation, policy->Data<BlockData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

    private:

       void CollectHandlers() override {
            using namespace srcDispatch;

            TryClausePolicy<CatchData, ParserState::catchstmt>::CollectHandlers();

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

    };

}

#endif
