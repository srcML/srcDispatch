// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OperatorPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_OPERATOR_POLICY_HPP
#define INCLUDED_OPERATOR_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <iostream>

namespace srcDispatch {

    struct OperatorData : public ElementData {

        DeltaElement<std::string> op;

        std::shared_ptr<OperatorData> copyAs(srcDispatch::DiffOperation operation) const {
            std::shared_ptr<OperatorData> data = std::make_shared<OperatorData>();
            data->op = DeltaElement<std::string>(operation, op.GetElement());
            return data;
        }

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return op.ToString(operation);
        }
    };

    class OperatorPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        OperatorData data;

    public:
        OperatorPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeOperatorPolicyHandlers();
        }

        ~OperatorPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<OperatorData>(data); }
        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeOperatorPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = OperatorData{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;
                CollectTokenHandlers();
            };

            // end of policy
            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeOperatorPolicyHandlers();
            };
        }

        void CollectTokenHandlers() {
            using namespace srcDispatch;
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                if(data.op.GetOperation() == srcDispatch::NONE) {
                    bool isChange = (ctx.depth + 1) == ctx.diffStack.back().depth && ctx.diffStack.back().isReplace;
                    srcDispatch::DiffOperation operation = isChange? CHANGE : ctx.diffStack.back().operation;
                    data.op = DeltaElement<std::string>(operation);
                }

                if(    ctx.diffStack.back().operation == srcDispatch::COMMON
                    || ctx.diffStack.back().operation == srcDispatch::DELETE) {
                    data.op.GetOriginal() += ctx.currentToken;
                } else if(ctx.diffStack.back().operation == srcDispatch::INSERT) {
                    data.op.GetModified() += ctx.currentToken;
                }
            };
        }
    };

}

#endif
