// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file SwitchPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SWITCH_POLICY_HPP
#define INCLUDED_SWITCH_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ConditionalPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct SwitchData : public ConditionalData {
    };

    class SwitchPolicy : public ConditionalPolicy<SwitchData, srcDispatch::ParserState::switchstmt> {
    public:
        SwitchPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<SwitchData, srcDispatch::ParserState::switchstmt>(listeners) {}
    };

}

#endif
