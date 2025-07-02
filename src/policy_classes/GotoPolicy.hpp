// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file GotoPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_GOTO_POLICY_HPP
#define INCLUDED_GOTO_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>

#include <NamePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct GotoData {
        enum GotoType { GOTO, BREAK, CONTINUE };

        unsigned int startLineNumber;
        unsigned int endLineNumber;

        DeltaElement<GotoType>                  type;
        DeltaElement<std::shared_ptr<NameData>> label;
    };

    // Collect the expression in the return
    class GotoPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener  {
    private:
        GotoData data;

        std::unique_ptr<NamePolicy> namePolicy;

    public:
        GotoPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeGotoPolicyHandlers();
        }

        ~GotoPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<GotoData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(NamePolicy) == typeid(*policy)) {
                data.label.Update(ctx.diffStack.back().operation, policy->Data<NameData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeGotoPolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            std::function<void(srcSAXEventContext& ctx)> startGoto = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = GotoData{};
                data.startLineNumber = ctx.startLineNumber;
                data.endLineNumber   = ctx.endLineNumber;

                if(ctx.currentTag == "break") {
                    data.type.Update(ctx.diffStack.back().operation, GotoData::BREAK);
                } else if(ctx.currentTag == "continue") {
                    data.type.Update(ctx.diffStack.back().operation, GotoData::CONTINUE);
                } else {
                    data.type.Update(ctx.diffStack.back().operation, GotoData::GOTO);
                }

                CollectLabelHandlers();
            };

            // end of policy
            std::function<void(srcSAXEventContext& ctx)> endGoto = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;
                depth = 0;
                NotifyAll(ctx);
                InitializeGotoPolicyHandlers();
            };

            openEventMap[ParserState::gotostmt]     = startGoto;
            openEventMap[ParserState::breakstmt]    = startGoto;
            openEventMap[ParserState::continuestmt] = startGoto;

            closeEventMap[ParserState::gotostmt]     = endGoto;
            closeEventMap[ParserState::breakstmt]    = endGoto;
            closeEventMap[ParserState::continuestmt] = endGoto;
        }

        void CollectLabelHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        }

    };

}

#endif
