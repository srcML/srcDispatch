// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file CasePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CASE_POLICY_HPP
#define INCLUDED_CASE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ExprTypePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct CaseData {
        DeltaElement<std::shared_ptr<ExpressionData>> expr;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return expr.ToString(operation);
        }
    };

    // Collect the expression in the return
    class CasePolicy : public ExprTypePolicy<CaseData, srcDispatch::ParserState::switchcase> {
    public:
        CasePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ExprTypePolicy<CaseData, srcDispatch::ParserState::switchcase>(listeners) {
        }

    };

}

#endif
