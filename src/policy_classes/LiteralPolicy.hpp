// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file LiteralPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_LITERAL_POLICY_HPP
#define INCLUDED_LITERAL_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <iostream>

namespace srcDispatch {

    struct LiteralData {

        unsigned int startLineNumber;
        unsigned int endLineNumber;

        DeltaElement<std::string> literal;

        std::shared_ptr<LiteralData> copyAs(srcDispatch::DiffOperation operation) const {
            std::shared_ptr<LiteralData> data = std::make_shared<LiteralData>();
            data->literal = DeltaElement<std::string>(operation, literal.GetElement());
            return data;
        }

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return literal.ToString(operation);
        }
    };

    class LiteralPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        LiteralData data;

    public:
        LiteralPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeLiteralPolicyHandlers();
        }

        ~LiteralPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<LiteralData>(data); }
        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {}
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeLiteralPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = LiteralData{};
                data.startLineNumber = ctx.startLineNumber;
                data.endLineNumber   = ctx.endLineNumber;
                CollectTokenHandlers();
            };

            // end of policy
            closeEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeLiteralPolicyHandlers();
            };
        }

        void CollectTokenHandlers() {
            using namespace srcDispatch;
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                if(data.literal.GetOperation() == srcDispatch::NONE) {
                    bool isChange = (ctx.depth + 1) == ctx.diffStack.back().depth && ctx.diffStack.back().isReplace;
                    srcDispatch::DiffOperation operation = isChange? CHANGE : ctx.diffStack.back().operation;
                    data.literal = DeltaElement<std::string>(operation);
                }

                if(    ctx.diffStack.back().operation == srcDispatch::COMMON
                    || ctx.diffStack.back().operation == srcDispatch::DELETE) {
                    data.literal.GetOriginal() += ctx.currentToken;
                } else if(ctx.diffStack.back().operation == srcDispatch::INSERT) {
                    data.literal.GetModified() += ctx.currentToken;
                }
            };
        }
    };

}

#endif
