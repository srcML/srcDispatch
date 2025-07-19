// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file GenericArgumentsPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#include <GenericArgumentsPolicy.hpp>
#include <DeltaElement.hpp>
#include <ExpressionPolicy.hpp>

#include <sstream>

namespace srcDispatch {

    std::string GenericArgumentsData::ToString(srcDispatch::DiffOperation operation) const {

        std::string str;

        bool hasArgument = false;
        bool printComma = false;
        for (const DeltaElement<std::shared_ptr<ExpressionData>>& argument : arguments) {

            bool outputRaw = argument.IsOfOperation(operation);
            if(outputRaw) {
                if(printComma) {
                    str += ", ";
                }
                if(!hasArgument) {
                    str += "<";
                    hasArgument = true;
                }
                printComma = true;
            }

            str += argument.ToString(operation);
        }

        if(hasArgument) {
            str += '>';
        }

        return str;
    }

    std::shared_ptr<GenericArgumentsData> GenericArgumentsData::copyAs(srcDispatch::DiffOperation operation) const {
        std::shared_ptr<GenericArgumentsData> data = std::make_shared<GenericArgumentsData>();
        data->startLineNumber = startLineNumber;
        data->endLineNumber   = endLineNumber;

        for (const DeltaElement<std::shared_ptr<ExpressionData>>& argument : arguments) {
            data->arguments.emplace_back(DeltaElement(operation, argument.GetElement()->copyAs(operation)));
        }

        return data;
    }

    GenericArgumentsPolicy::GenericArgumentsPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners), data{} {
        InitializeGenericArgumentsPolicyHandlers();
    }

    GenericArgumentsPolicy::~GenericArgumentsPolicy() {}

    void GenericArgumentsPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if(typeid(NamePolicy) == typeid(*policy)) {
            //data.arguments.push_back(DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>()));
            std::shared_ptr<NameData> nameData = policy->Data<NameData>();
            ExpressionData exprData;
            exprData.startLineNumber.Update(nameData->startLineNumber.GetOperation(), nameData->startLineNumber.GetElement());
            exprData.endLineNumber.Update(nameData->endLineNumber.GetOperation(), nameData->endLineNumber.GetElement());
            DeltaElement<std::any> exprAny(ctx.diffStack.back().operation, nameData);
            exprData.expr.emplace_back(exprAny);
            data.arguments.push_back(DeltaElement(ctx.diffStack.back().operation, std::make_shared<ExpressionData>(exprData)));
        } else if(typeid(ExpressionPolicy) == typeid(*policy)) {
            data.arguments.push_back(DeltaElement(ctx.diffStack.back().operation, policy->Data<ExpressionData>()));
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
        ctx.dispatcher->RemoveListener(nullptr);
    }

    std::any GenericArgumentsPolicy::DataInner() const {
        return std::make_shared<GenericArgumentsData>(data);
    }

    void GenericArgumentsPolicy::InitializeGenericArgumentsPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext &ctx) {
            if(depth) return;

            depth = ctx.depth;
            data = GenericArgumentsData{};
            data.startLineNumber = ctx.startLineNumber;
            data.endLineNumber   = ctx.endLineNumber;
            CollectArgumentHandler();
        };

        // end of policy
        closeEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext &ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeGenericArgumentsPolicyHandlers();
        };

        openEventMap[ParserState::genericparameterlist] = [this](srcSAXEventContext &ctx) {
            if(depth) return;

            depth = ctx.depth;
            data = GenericArgumentsData{};
            data.startLineNumber = ctx.startLineNumber;
            data.endLineNumber = ctx.endLineNumber;
            CollectParameterHandler();
        };

        closeEventMap[ParserState::genericparameterlist] = [this](srcSAXEventContext &ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeGenericArgumentsPolicyHandlers();
        };
    }

    void GenericArgumentsPolicy::CollectArgumentHandler() {
        using namespace srcDispatch;

        openEventMap[ParserState::argument] = [this](srcSAXEventContext &ctx) {
            if(!depth) return;

            openEventMap[ParserState::expr] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!expressionPolicy) {
                    expressionPolicy = make_unique_policy<ExpressionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(expressionPolicy.get());
            };

            openEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        };

        closeEventMap[ParserState::argument] = [this](srcSAXEventContext &ctx) {
            if(!depth) return;

            NopOpenEvents({ParserState::expr, ParserState::name});
        };
    }

    void GenericArgumentsPolicy::CollectParameterHandler() {
        using namespace srcDispatch;

        openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        };
        closeEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            NopOpenEvents({ParserState::name});
        };
    }

}
