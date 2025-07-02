// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ExpressionPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#include <ExpressionPolicy.hpp>
#include <DeltaElement.hpp>

namespace srcDispatch {

    std::string ExpressionData::ToString(srcDispatch::DiffOperation operation) const {

        std::string str = "";
        bool outputSpace = false;
        for (const DeltaElement<std::any>& item : expr) {

            bool outputRaw = item.IsOfOperation(operation);
            if(outputRaw) {
                if(outputSpace) {
                    str += ' ';
                }
                outputSpace = true;
            }

            const std::type_info& type = item.HasOriginal()? item.GetOriginal().type() : item.GetModified().type();
            if(type == typeid(std::shared_ptr<NameData>)) {
                str += item.ToString<std::shared_ptr<NameData>>(operation);
            } else if(type == typeid(std::shared_ptr<OperatorData>)) {
                str += item.ToString<std::shared_ptr<OperatorData>>(operation);
            } else if(type == typeid(std::shared_ptr<LiteralData>)) {
                str += item.ToString<std::shared_ptr<LiteralData>>(operation);
            } else if(type == typeid(std::shared_ptr<CallData>)) {
                str += item.ToString<std::shared_ptr<CallData>>(operation);
            }
        }

        return str;    
    }

    std::shared_ptr<ExpressionData> ExpressionData::copyAs(srcDispatch::DiffOperation operation) const {
        std::shared_ptr<ExpressionData> data = std::make_shared<ExpressionData>();
        data->startLineNumber = startLineNumber;
        data->endLineNumber   = endLineNumber;

        for (const DeltaElement<std::any>& item : expr) {
            DeltaElement<std::any> exprAny;
            if(item.GetElement().type() == typeid(std::shared_ptr<NameData>)) {
                exprAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<NameData>>(item)->copyAs(operation));
            } else if(item.GetElement().type() == typeid(std::shared_ptr<OperatorData>)) {
                exprAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<OperatorData>>(item)->copyAs(operation));
            } else if(item.GetElement().type() == typeid(std::shared_ptr<LiteralData>)) {
                exprAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<LiteralData>>(item)->copyAs(operation));
            } else {
                exprAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<CallData>>(item)->copyAs(operation));
            }

            data->expr.emplace_back(exprAny);
        }

        return data;
    }

    ExpressionPolicy::~ExpressionPolicy() {}

    void ExpressionPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if(typeid(NamePolicy) == typeid(*policy)) {
            data.expr.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<NameData>()));
        } else if(typeid(OperatorPolicy) == typeid(*policy)) {
            data.expr.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<OperatorData>()));
        } else if(typeid(LiteralPolicy) == typeid(*policy)) {
            data.expr.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<LiteralData>()));
        } else if(typeid(CallPolicy) == typeid(*policy)) {
            data.expr.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<CallData>()));
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        // Operators are added in CollectOtherHandlers()
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    }

    void ExpressionPolicy::InitializeExpressionPolicyHandlers()
    {
        using namespace srcDispatch;
        // start of policy
        std::function<void(srcSAXEventContext& ctx)> expressionStart = [this](srcSAXEventContext& ctx) {
            if(depth) return;

            depth = ctx.depth;
            data = ExpressionData{};
            data.startLineNumber = ctx.startLineNumber;
            data.endLineNumber   = ctx.endLineNumber;
            CollectNameHandlers();
            CollectCallHandlers();
            CollectOperatorHandlers();
            CollectLiteralHandlers();
        };

        // end of policy
        std::function<void(srcSAXEventContext& ctx)> expressionEnd = [this](srcSAXEventContext& ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeExpressionPolicyHandlers();
        };

        openEventMap[ParserState::expr] = expressionStart;
        closeEventMap[ParserState::expr] = expressionEnd;
    }

    void ExpressionPolicy::CollectNameHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!namePolicy) {
                namePolicy = make_unique_policy<NamePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(namePolicy.get());
        };
    }

    void ExpressionPolicy::CollectCallHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!callPolicy) {
                callPolicy = make_unique_policy<CallPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(callPolicy.get());
        };
    }

    void ExpressionPolicy::CollectOperatorHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!operatorPolicy) {
                operatorPolicy = make_unique_policy<OperatorPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(operatorPolicy.get());
        };
    }

    void ExpressionPolicy::CollectLiteralHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!literalPolicy) {
                literalPolicy = make_unique_policy<LiteralPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(literalPolicy.get());
        };
    }

}
