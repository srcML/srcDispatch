// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffCasePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_CASE_POLICY_HPP
#define INCLUDED_SRCDIFF_CASE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <srcDiffExprTypePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDiffDispatch {

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
