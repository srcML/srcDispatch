// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file GenericArgumentsPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_GENERIC_ARGUMENTS_POLICY_HPP
#define INCLUDED_GENERIC_ARGUMENTS_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <NamePolicy.hpp>

namespace srcDispatch {

    class NamePolicy;
    class ExpressionPolicy;
    struct ExpressionData;

    struct GenericArgumentsData : public ElementData {

        std::vector<DeltaElement<std::shared_ptr<ExpressionData>>> arguments;

        std::shared_ptr<GenericArgumentsData> copyAs(DiffOperation operation) const;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(DiffOperation operation) const;
    };

    class GenericArgumentsPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {


    private:
        GenericArgumentsData data;

        std::unique_ptr<ExpressionPolicy> expressionPolicy;

    public:
        GenericArgumentsPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~GenericArgumentsPolicy();
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    protected:
        virtual std::any DataInner() const override;

    private:
        void InitializeGenericArgumentsPolicyHandlers();
        void CollectArgumentHandler();
    };

}

#endif
