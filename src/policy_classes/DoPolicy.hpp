// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffDoPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_DO_POLICY_HPP
#define INCLUDED_SRCDIFF_DO_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicy.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDiffDispatch {

    struct DoData {

        unsigned int startLineNumber;
        unsigned int endLineNumber;

        DeltaElement<std::shared_ptr<ConditionData>> condition;
        DeltaElement<std::shared_ptr<BlockData>>     block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return condition.ToString(operation);
        }
    };

    class DoPolicy : public ConditionalPolicy<DoData, srcDispatch::ParserState::dostmt> {
    public:
        DoPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<DoData, srcDispatch::ParserState::dostmt>(listeners) {}
    };

}

#endif
