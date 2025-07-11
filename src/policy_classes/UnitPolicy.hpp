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
#ifndef INCLUDED_UNIT_POLICY_HPP
#define INCLUDED_UNIT_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <DeclStmtPolicy.hpp>
#include <FunctionPolicy.hpp>
#include <ClassPolicy.hpp>
#include <DeltaElement.hpp>

#include <memory>
#include <typeinfo>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

    struct UnitData {
        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

        std::vector<DeltaElement<std::shared_ptr<ClassData>>>    classInfo;
        std::vector<DeltaElement<std::shared_ptr<FunctionData>>> functionInfo;
        std::vector<DeltaElement<std::shared_ptr<DeclStmtData>>> declStmtInfo;
    };

    class UnitPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    public:
        static const std::size_t MAX_DEPTH = (std::size_t)-1;

        UnitData data;
        std::size_t unitDepth;

        std::unique_ptr<DeclStmtPolicy> declStmtPolicy;
        std::unique_ptr<FunctionPolicy> functionPolicy;
        std::unique_ptr<ClassPolicy>    classPolicy;

    public:
        UnitPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners) 
            : srcDispatch::PolicyDispatcher(listeners), unitDepth(MAX_DEPTH) {
            InitializeUnitPolicyHandlers();
        }

        ~UnitPolicy() {}

        void Notify(const srcDispatch::PolicyDispatcher* policy,
                    const srcDispatch::srcSAXEventContext&  ctx) override {
            // Save class and function information
            if(typeid(ClassPolicy) == typeid(*policy)) {
                srcDispatch::DiffOperation operation = ctx.diffStack.back().isConvert? srcDispatch::COMMON : ctx.diffStack.back().operation;
                data.classInfo.emplace_back(operation, policy->Data<ClassData>());
            } else if(typeid(FunctionPolicy) == typeid(*policy)) {
                data.functionInfo.emplace_back(ctx.diffStack.back().operation, policy->Data<FunctionData>());
            } else if(typeid(DeclStmtPolicy) == typeid(*policy)) {
                data.declStmtInfo.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclStmtData>());
            }
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    protected:
        std::any DataInner() const override { return std::make_shared<UnitData>(data); }

    private:
        void InitializeUnitPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::unit] = [this](srcSAXEventContext& ctx) {
                if(unitDepth == MAX_DEPTH && (ctx.isArchive || ctx.depth > 0)) {
                    unitDepth = ctx.depth;
                    data = UnitData{};
                    data.startLineNumber = ctx.startLineNumber;
                    data.endLineNumber   = ctx.endLineNumber;
                }
            };

            closeEventMap[ParserState::unit] = [this](srcSAXEventContext& ctx) {
                if(unitDepth != ctx.depth) return;

                unitDepth = MAX_DEPTH;
                NotifyAll(ctx);
            };

            // start of policy
            std::function<void(srcDispatch::srcSAXEventContext& )> startClassPolicy = [this](srcSAXEventContext& ctx) {
                if(depth == MAX_DEPTH) return;

                if(!classPolicy) {
                    classPolicy = make_unique_policy<ClassPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(classPolicy.get());
            };

            openEventMap[ParserState::classn]     = startClassPolicy;
            openEventMap[ParserState::structn]    = startClassPolicy;
            openEventMap[ParserState::interfacen] = startClassPolicy;

            // end of policy
            std::function<void(srcDispatch::srcSAXEventContext& )> endClassPolicy = [](srcSAXEventContext& ctx) {};

            closeEventMap[ParserState::classn]     = endClassPolicy;
            closeEventMap[ParserState::structn]    = endClassPolicy;
            closeEventMap[ParserState::interfacen] = endClassPolicy;

            // start function of policy
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

            openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
                if(depth == MAX_DEPTH) return;

                if(!declStmtPolicy) {
                    declStmtPolicy = make_unique_policy<DeclStmtPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(declStmtPolicy.get());
            };

            closeEventMap[ParserState::declstmt] = [](srcSAXEventContext& ctx) {};
        }
    };

}

#endif
