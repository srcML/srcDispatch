// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ElsePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_ELSE_POLICY_HPP
#define INCLUDED_ELSE_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ConditionalPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ElseData : public ConditionalData {
    };

    class ElsePolicy : public ConditionalPolicy<ElseData, srcDispatch::ParserState::elsestmt> {
    public:
        ElsePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<ElseData, srcDispatch::ParserState::elsestmt>(listeners) {}
    };

}

#endif
