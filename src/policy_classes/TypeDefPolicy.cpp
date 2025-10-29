// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TypeDefPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#include <TypeDefPolicy.hpp>

#include <FunctionPolicy.hpp>

namespace srcDispatch {

TypeDefPolicy::TypeDefPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
    : srcDispatch::PolicyDispatcher(listeners), data{} {
    InitializeTypeDefPolicyHandlers();
}

TypeDefPolicy::~TypeDefPolicy() {}

std::any TypeDefPolicy::DataInner() const { return std::make_shared<TypeDefData>(data); }

void TypeDefPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    if(typeid(TypePolicy) == typeid(*policy)) {
        data.type = DeltaElement(ctx.diffStack.back().operation, policy->Data<TypeData>());
    } else if(typeid(NamePolicy) == typeid(*policy)) {
        data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
    } else if(typeid(FunctionPolicy) == typeid(*policy)) {
        data.functionDecl = DeltaElement(ctx.diffStack.back().operation, policy->Data<FunctionData>());
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }
    ctx.dispatcher->RemoveListener(nullptr);
}

void TypeDefPolicy::NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) {}

void TypeDefPolicy::InitializeTypeDefPolicyHandlers() {
    using namespace srcDispatch;

    // start of policy
    openEventMap[ParserState::typedefdecl] = [this](srcSAXEventContext &ctx) {
        if(depth) return;

        depth = ctx.depth;
        data = TypeDefData{};
        data.startPosition = ctx.startPosition;
        data.endPosition   = ctx.endPosition;                    
        CollectTypeHandlers(ctx);
        CollectNameHandlers(ctx);
        CollectFunctionDeclHandlers(ctx);
    };

    // end of policy
    closeEventMap[ParserState::typedefdecl] = [this](srcSAXEventContext &ctx) {
        if(!depth || depth != ctx.depth) return;

        depth = 0;
        NotifyAll(ctx);
        data = TypeDefData{};
        InitializeTypeDefPolicyHandlers();
    };
}

void TypeDefPolicy::CollectTypeHandlers(srcDispatch::srcSAXEventContext& ctx) {
    using namespace srcDispatch;

    openEventMap[ParserState::type] = [this](srcSAXEventContext &ctx) {
        if(!depth) return;

        if(!typePolicy) {
            typePolicy = make_unique_policy<TypePolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(typePolicy.get());
    };
}

void TypeDefPolicy::CollectNameHandlers(srcDispatch::srcSAXEventContext& ctx) {
    using namespace srcDispatch;

    openEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
        if(!depth) return;

        if(!namePolicy) {
            namePolicy = make_unique_policy<NamePolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(namePolicy.get());
    };
}

void TypeDefPolicy::CollectFunctionDeclHandlers(srcDispatch::srcSAXEventContext& ctx) {
    using namespace srcDispatch;

    openEventMap[ParserState::functiondecl] = [this](srcSAXEventContext &ctx) {
        if(!depth) return;

        if(!functionPolicy) {
            functionPolicy = make_unique_policy<FunctionPolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(functionPolicy.get());
    };
}

}