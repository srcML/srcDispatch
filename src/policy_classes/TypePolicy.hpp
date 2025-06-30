// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TypePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_TYPE_POLICY_HPP
#define INCLUDED_TYPE_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <NamePolicy.hpp>
#include <DeltaElement.hpp>

#include <exception>

namespace srcDispatch {

    class NamePolicy;

    struct TypeData {
        enum TypeType : int { TYPENAME, POINTER, REFERENCE, RVALUE, SPECIFIER, NONE };

        unsigned int lineNumber;
        std::vector<std::pair<DeltaElement<std::any>, DeltaElement<TypeType>>> types;

        std::shared_ptr<TypeData> copyAs(srcDispatch::DiffOperation operation) const;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const;
    };

    class TypePolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        TypeData data;

        std::unique_ptr<NamePolicy> namePolicy;

    public:
        TypePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~TypePolicy();
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    protected:
        virtual std::any DataInner() const override;

    private:
        void InitializeTypePolicyHandlers();
        void CollectNamesHandler();
        void CollectModifersHandler();
        void CollectSpecifiersHandler();
    };
}

#endif
