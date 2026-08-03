// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DoPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_DO_POLICY_HPP
#define INCLUDED_DO_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicy.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct DoData : public ConditionalData {
    };

    class DoPolicy : public ConditionalPolicy<DoData, srcDispatch::ParserState::dostmt> {
    public:
        DoPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
            : ConditionalPolicy<DoData, srcDispatch::ParserState::dostmt>(listeners) {}
    };

}

#endif
