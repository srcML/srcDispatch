// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file BlockPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_BLOCK_POLICY_HPP
#define INCLUDED_BLOCK_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <DeclStmtPolicy.hpp>
#include <TypedefPolicy.hpp>
#include <ExprStmtPolicy.hpp>
#include <ReturnPolicy.hpp>
#include <ThrowPolicy.hpp>
#include <CasePolicy.hpp>
#include <GotoPolicy.hpp>
#include <LabelPolicy.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

namespace srcDispatch {

    class IfStmtPolicy;
    class SwitchPolicy;
    class WhilePolicy;
    class ForPolicy;
    class DoPolicy;
    class TryPolicy;

    class  ClassPolicy;
    struct ClassData;

    class ConvertPlexerPolicy;

    struct BlockData : public ElementData {

        std::vector<DeltaElement<std::any>>                   statements;
        std::vector<DeltaElement<std::shared_ptr<ClassData>>> localClasses;
        std::vector<DeltaElement<std::shared_ptr<LabelData>>> labels;
        std::vector<DeltaElement<std::shared_ptr<CaseData>>>  cases;
        std::vector<DeltaElement<std::shared_ptr<BlockData>>> blocks;

    };

    class BlockPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        BlockData data;

        std::unique_ptr<BlockPolicy>    blockPolicy;

        std::unique_ptr<DeclStmtPolicy> declstmtPolicy;
        std::unique_ptr<TypedefPolicy>  typedefPolicy;
        std::unique_ptr<ExprStmtPolicy> exprStmtPolicy;
        std::unique_ptr<ReturnPolicy>   returnPolicy;

        std::unique_ptr<GotoPolicy>     gotoPolicy;
        std::unique_ptr<LabelPolicy>    labelPolicy;
        std::unique_ptr<ThrowPolicy>    throwPolicy;

        std::unique_ptr<IfStmtPolicy>   ifStmtPolicy;

        std::unique_ptr<ForPolicy>      forPolicy;
        std::unique_ptr<WhilePolicy>    whilePolicy;
        std::unique_ptr<DoPolicy>       doPolicy;

        std::unique_ptr<TryPolicy>      tryPolicy;

        std::unique_ptr<SwitchPolicy>   switchPolicy;
        std::unique_ptr<CasePolicy>     casePolicy;

        std::unique_ptr<ClassPolicy>    classPolicy;

        std::unique_ptr<ConvertPlexerPolicy> plexer;

    public:
        BlockPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);

        ~BlockPolicy();

    protected:
        std::any DataInner() const override;
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeBlockPolicyHandlers();

        void CollectBlockHandlers();
        void CollectDeclStmtHandlers();
        void CollectTypedefHandlers();
        void CollectExpressionHandlers();
        void CollectReturnHandlers();
        void CollectIfStmtHandlers();
        void CollectSwitchHandlers();
        void CollectWhileHandlers();
        void CollectForHandlers();
        void CollectDoHandlers();
        void CollectTryHandlers();
        void CollectThrowHandlers();
        void CollectGotoHandlers();
        void CollectClassHandlers();
        void CollectCaseHandlers();
        void CollectLabelHandlers();

        template<typename type>
        bool ConvertRegistrationCheck(srcDispatch::srcSAXEventContext& ctx);
    };

}

#endif
