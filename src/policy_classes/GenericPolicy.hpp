// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file GenericPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_GENERIC_POLICY_HPP
#define INCLUDED_GENERIC_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <DeltaElement.hpp>


namespace srcDispatch {

    class  DeclPolicy;
    struct DeclData;

    class  NamePolicy;

    class  ExpressionPolicy;
    struct ExpressionData;

    struct GenericData {

        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

        std::vector<DeltaElement<std::shared_ptr<DeclData>>> parameters;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const;
    };

    class GenericPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        GenericData data;

        std::unique_ptr<DeclPolicy> declPolicy;

    public:
        GenericPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~GenericPolicy();
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    protected:
        virtual std::any DataInner() const override;

    private:
        void InitializeGenericPolicyHandlers();
        void CollectParameterHandlers();
    };

}

#endif
