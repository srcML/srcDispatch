// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TypeDefPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_TYPE_DEF_POLICY_HPP
#define INCLUDED_TYPE_DEF_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <TypePolicy.hpp>
#include <NamePolicy.hpp>

#include <string>
#include <vector>

namespace srcDispatch {

    struct FunctionData;
    class FunctionPolicy;

     struct TypeDefData : public ElementData {

        DeltaElement<std::shared_ptr<TypeData>> type;
        DeltaElement<std::shared_ptr<NameData>> name;

        // Consider merging with type and extracting name out of it
        DeltaElement<std::shared_ptr<FunctionData>> functionDecl;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            std::string str;
            if(type) {
                str += type.ToString(operation);
            }

            if(functionDecl) {
                str += functionDecl.ToString(operation);
            }

            if(name) {
                str += ' ';
                str += name.ToString(operation);
            }

            return str;
        }
    };

    class TypeDefPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        TypeDefData data;

        std::unique_ptr<TypePolicy> typePolicy;
        std::unique_ptr<NamePolicy> namePolicy;

        std::unique_ptr<FunctionPolicy> functionPolicy;

    public:
        TypeDefPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
        ~TypeDefPolicy();

    protected:
        std::any DataInner() const override;

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override;

    private:
        void InitializeTypeDefPolicyHandlers();

        void CollectTypeHandlers(srcDispatch::srcSAXEventContext& ctx);
        void CollectNameHandlers(srcDispatch::srcSAXEventContext& ctx);
        void CollectFunctionDeclHandlers(srcDispatch::srcSAXEventContext& ctx);
};

}

#endif
