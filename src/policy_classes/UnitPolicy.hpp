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

namespace srcDispatch {

    struct UnitData {
        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

        std::vector<DeltaElement<std::shared_ptr<DeclStmtData>>> declStmts;
        std::vector<DeltaElement<std::shared_ptr<ClassData>>>    classes;
        std::vector<DeltaElement<std::shared_ptr<FunctionData>>> functions;
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
                    const srcDispatch::srcSAXEventContext&  ctx) override;

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    protected:
        std::any DataInner() const override { return std::make_shared<UnitData>(data); }

    private:
        void InitializeUnitPolicyHandlers();
        void InitializeDeclStmtHandlers();
        void InitializeClassHandlers();
        void InitializeFunctionHandlers();

    };

}

#endif
