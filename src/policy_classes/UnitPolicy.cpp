// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file UnitPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 *
 * Policy for srcDispatch
 * Listens for both classes and functions
 * Calls the ClassPolicy for class
 * Calls the FunctionPolicy for function
 *
 */

#include <UnitPolicy.hpp>

#include <memory>
#include <typeinfo>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

void UnitPolicy::Notify(const srcDispatch::PolicyDispatcher* policy,
            const srcDispatch::srcSAXEventContext&  ctx) {

    if(typeid(DeclStmtPolicy) == typeid(*policy)) {
        data.declStmts.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclStmtData>());
    } else if(typeid(ClassPolicy) == typeid(*policy)) {
        srcDispatch::DiffOperation operation = ctx.diffStack.back().isConvert? srcDispatch::COMMON : ctx.diffStack.back().operation;
        data.classes.emplace_back(operation, policy->Data<ClassData>());
    } else if(typeid(FunctionPolicy) == typeid(*policy)) {
        data.functions.emplace_back(ctx.diffStack.back().operation, policy->Data<FunctionData>());
    }
    ctx.dispatcher->RemoveListenerDispatch(nullptr);
}

void UnitPolicy::InitializeUnitPolicyHandlers() {
    using namespace srcDispatch;

    openEventMap[ParserState::unit] = [this](srcSAXEventContext& ctx) {
        if(unitDepth == MAX_DEPTH && (ctx.isArchive || ctx.depth > 0)) {
            unitDepth = ctx.depth;
            data = UnitData{};
            data.startLineNumber = ctx.startLineNumber;
            data.endLineNumber   = ctx.endLineNumber;

            InitializeDeclStmtHandlers();
            InitializeClassHandlers();
            InitializeFunctionHandlers();
        }
    };

    closeEventMap[ParserState::unit] = [this](srcSAXEventContext& ctx) {
        if(unitDepth != ctx.depth) return;

        unitDepth = MAX_DEPTH;
        InitializeUnitPolicyHandlers();
        NotifyAll(ctx);
    };

}

void UnitPolicy::InitializeDeclStmtHandlers() {
    openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
        if(depth == MAX_DEPTH) return;

        if(!declStmtPolicy) {
            declStmtPolicy = make_unique_policy<DeclStmtPolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(declStmtPolicy.get());
    };

    closeEventMap[ParserState::declstmt] = [](srcSAXEventContext& ctx) {};
}

void UnitPolicy::InitializeClassHandlers() {
    using namespace srcDispatch;

    std::function<void(srcDispatch::srcSAXEventContext& )> startClassPolicy = [this](srcSAXEventContext& ctx) {
        if(depth == MAX_DEPTH) return;

        if(!classPolicy) {
            classPolicy = make_unique_policy<ClassPolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(classPolicy.get());
    };

    openEventMap[ParserState::classn]  = startClassPolicy;
    openEventMap[ParserState::structn] = startClassPolicy;

    // end of policy
    std::function<void(srcDispatch::srcSAXEventContext& )> endClassPolicy = [](srcSAXEventContext& ctx) {};

    closeEventMap[ParserState::classn]  = endClassPolicy;
    closeEventMap[ParserState::structn] = endClassPolicy;
}

void UnitPolicy::InitializeFunctionHandlers() {
    std::function<void(srcDispatch::srcSAXEventContext& )> startFunction = [this](srcSAXEventContext& ctx) {
        if(depth == MAX_DEPTH) return;

        if(!functionPolicy) {
            functionPolicy = make_unique_policy<FunctionPolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(functionPolicy.get());
    };

    openEventMap[ParserState::function]        = startFunction;
    openEventMap[ParserState::functiondecl]    = startFunction;
    openEventMap[ParserState::constructor]     = startFunction;
    openEventMap[ParserState::constructordecl] = startFunction;
    openEventMap[ParserState::destructor]      = startFunction;
    openEventMap[ParserState::destructordecl]  = startFunction;

    // end of policy
    std::function<void(srcDispatch::srcSAXEventContext& )> endFunction = [](srcSAXEventContext& ctx) {};

    closeEventMap[ParserState::function]        = endFunction;
    closeEventMap[ParserState::functiondecl]    = endFunction;
    closeEventMap[ParserState::constructor]     = endFunction;
    closeEventMap[ParserState::constructordecl] = endFunction;
    closeEventMap[ParserState::destructor]      = endFunction;
    closeEventMap[ParserState::destructordecl]  = endFunction;
}

}
