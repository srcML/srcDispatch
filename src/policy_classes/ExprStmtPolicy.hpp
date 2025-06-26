// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ExprStmtPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www..org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_EXPR_STMT_POLICY_HPP
#define INCLUDED_SRCDIFF_EXPR_STMT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <ExprTypePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ExprStmtData {
        DeltaElement<std::shared_ptr<ExpressionData>> expr;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return expr.ToString(operation);
        }
    };

    class ExprStmtPolicy : public ExprTypePolicy<ExprStmtData, srcDispatch::ParserState::exprstmt> {
    public:
        ExprStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ExprTypePolicy<ExprStmtData, srcDispatch::ParserState::exprstmt>(listeners) {
        }

    };

}

#endif
