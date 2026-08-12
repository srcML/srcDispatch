// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file FinallyPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_FINALLY_POLICY_HPP
#define INCLUDED_FINALLY_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct FinallyData : public ElementData {
        DeltaElement<std::shared_ptr<BlockData>> block;
    };

    class FinallyPolicy : public TryClausePolicy<FinallyData, srcDispatch::ParserState::finallystmt> {

    private:
        FinallyData   data;

    public:
        FinallyPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : TryClausePolicy<FinallyData, srcDispatch::ParserState::finallystmt>(listeners), data{} {}

        ~FinallyPolicy() {}

    };

}

#endif
