// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffGenericPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#include <srcDiffGenericPolicy.hpp>
#include <DeltaElement.hpp>

#include <srcDiffDeclPolicy.hpp>
#include <srcDiffExpressionPolicy.hpp>

#include <sstream>

namespace srcDiffDispatch {

    std::string GenericData::ToString(srcDispatch::DiffOperation operation) const {

        std::string str;

        bool hasParameter = false;
        bool printComma = false;
        for (const DeltaElement<std::shared_ptr<DeclData>>& parameter : parameters) {

            bool outputRaw = parameter.IsOfOperation(operation);
            if(outputRaw) {
                if(printComma) {
                    str += ", ";
                }
                if(!hasParameter) {
                    str += "template<";
                    hasParameter = true;
                }
                printComma = true;
            }

            str += parameter.ToString(operation);
        }

        if(hasParameter) {
            str += '>';
        }

        return str;
    }

    GenericPolicy::GenericPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners), data{} {
        InitializeGenericPolicyHandlers();
    }

    GenericPolicy::~GenericPolicy() {}

    void GenericPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if(typeid(DeclPolicy) == typeid(*policy)) {
            data.parameters.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclData>());
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
        ctx.dispatcher->RemoveListener(nullptr);
    }

    std::any GenericPolicy::DataInner() const {
        return std::make_shared<GenericData>(data);
    }

    void GenericPolicy::InitializeGenericPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::templates] = [this](srcSAXEventContext &ctx) {
            if(!depth) {
                depth = ctx.depth;
                data = GenericData{};
                data.lineNumber = ctx.startLineNumber;
                CollectParameterHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::templates] = [this](srcSAXEventContext &ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeGenericPolicyHandlers();
        };
    }

    void GenericPolicy::CollectParameterHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
                if(!declPolicy) {
                    declPolicy = make_unique_policy<DeclPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(declPolicy.get());
            };
        };

        closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            NopOpenEvents({ParserState::parameter});
        };
    }
}
