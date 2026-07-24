// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NamePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_NAME_POLICY_HPP
#define INCLUDED_NAME_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <GenericArgumentsPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class ExpressionPolicy;
    struct ExpressionData;

    class GenericArgumentsPolicy;
    struct GenericArgumentsData;

    struct NameData : public ElementData {

        DeltaElement<std::string> name;
        std::vector<DeltaElement<std::any>> names;
        DeltaElement<std::shared_ptr<GenericArgumentsData>> templateArgumentList;
        std::vector<DeltaElement<std::shared_ptr<ExpressionData>>> indices;

        std::string SimpleName() const;
        std::shared_ptr<NameData> copyAs(srcDispatch::DiffOperation operation) const;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const;
    };

    class NamePolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        NameData data;

        std::unique_ptr<NamePolicy> namePolicy;
        std::unique_ptr<OperatorPolicy> operatorPolicy;
        std::unique_ptr<GenericArgumentsPolicy> templateArgumentListPolicy;
        std::unique_ptr<ExpressionPolicy> expressionPolicy;

    public:
        NamePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);

        ~NamePolicy();

    protected:
        std::any DataInner() const override { return std::make_shared<NameData>(data); }
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeNamePolicyHandlers();
        void CollectOperatorsHandlers();
        void CollectGenericArgumentsHandlers();
        void CollectArrayIndicesHandlers();
    };

}

#endif
