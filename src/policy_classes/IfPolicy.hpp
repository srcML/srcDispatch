// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file IfPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_IF_POLICY_HPP
#define INCLUDED_IF_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ConditionalPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct IfData : public ConditionalData {
    };

    class IfPolicy : public ConditionalPolicy<IfData, srcDispatch::ParserState::ifstmt> {
    public:
        IfPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<IfData, srcDispatch::ParserState::ifstmt>(listeners) {}
    };

}

#endif
