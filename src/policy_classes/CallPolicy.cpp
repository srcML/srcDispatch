// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffCallPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#include <CallPolicy.hpp>
#include <DeltaElement.hpp>

namespace srcDiffDispatch {

    std::string CallData::ToString(srcDispatch::DiffOperation operation) const {

        std::string str = name.ToString(operation);

        if(name.IsOfOperation(operation)) {
            str += '(';
        }

        bool printComma = false;
        for (const DeltaElement<std::shared_ptr<ExpressionData>>& arg : arguments) {

            bool outputRaw = arg.IsOfOperation(operation);
            if(outputRaw) {
              if(printComma) {
                    str += ", ";
                }
                printComma = true;
            }
            str += arg.ToString(operation);

        }

        if(name.IsOfOperation(operation)) {
            str += ')';
        }

        return str;
    }

    std::shared_ptr<CallData> CallData::copyAs(srcDispatch::DiffOperation operation) const {
        std::shared_ptr<CallData> data = std::make_shared<CallData>();
        data->lineNumber = lineNumber;

        data->name = name.GetElement()->copyAs(operation);

        for (const DeltaElement<std::shared_ptr<ExpressionData>>& argument : arguments) {
            data->arguments.emplace_back(DeltaElement(operation, argument.GetElement()->copyAs(operation)));
        }

        return data;
    }

    CallPolicy::~CallPolicy() {}

    std::any CallPolicy::DataInner() const { return std::make_shared<CallData>(data); }

    void CallPolicy::Notify(const srcDispatch::PolicyDispatcher *policy, const srcDispatch::srcSAXEventContext& ctx) {
        using namespace srcDispatch;
        if(typeid(NamePolicy) == typeid(*policy)) {
            data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
        } else if(typeid(ExpressionPolicy) == typeid(*policy)) {
            data.arguments.push_back(DeltaElement(ctx.diffStack.back().operation, policy->Data<ExpressionData>()));
        } else {
            throw PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void CallPolicy::InitializeCallPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
            if(!depth) {
                depth = ctx.depth;
                data = CallData{};
                data.lineNumber = ctx.startLineNumber;
                CollectNameHandlers();
                CollectCallArgumentHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeCallPolicyHandlers();
        };
    }

    void CallPolicy::CollectNameHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!namePolicy) {
                namePolicy = make_unique_policy<NamePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(namePolicy.get());
        };
    }

    void CallPolicy::CollectCallArgumentHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!expressionPolicy) {
                expressionPolicy = make_unique_policy<ExpressionPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(expressionPolicy.get());
        };
    }

}
