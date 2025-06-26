// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ReturnPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www..org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_RETURN_POLICY_HPP
#define INCLUDED_SRCDIFF_RETURN_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ExprTypePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ReturnData {
        DeltaElement<std::shared_ptr<ExpressionData>> expr;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return expr.ToString(operation);
        }
    };

    // Collect the expression in the return
    class ReturnPolicy : public ExprTypePolicy<ReturnData, srcDispatch::ParserState::returnstmt> {
    public:
        ReturnPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ExprTypePolicy<ReturnData, srcDispatch::ParserState::returnstmt>(listeners) {
        }

    };

}

#endif
