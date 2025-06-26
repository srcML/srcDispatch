// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file CallPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CALL_POLICY_HPP
#define INCLUDED_CALL_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <NamePolicy.hpp>
#include <ExpressionPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

//
// Collects <call> information
//  foo(x)
//  obj.foo(x)
//
//  Gets the name and list of arguments
//

namespace srcDispatch {

    struct ExpressionData;
    class ExpressionPolicy;

    struct NameData;
    class NamePolicy;

    struct CallData {

        unsigned int lineNumber;
        DeltaElement<std::shared_ptr<NameData>> name;
        std::vector<DeltaElement<std::shared_ptr<ExpressionData>>> arguments; // expressions

        std::shared_ptr<CallData> copyAs(srcDispatch::DiffOperation operation) const;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const;
    };

    class CallPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        CallData data;

        std::unique_ptr<NamePolicy> namePolicy;
        std::unique_ptr<ExpressionPolicy> expressionPolicy;

    public:
        CallPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeCallPolicyHandlers();
        }

        ~CallPolicy();

    protected:
        std::any DataInner() const override;
        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeCallPolicyHandlers();
        void CollectNameHandlers();
        void CollectCallArgumentHandlers();
    };

}

#endif
