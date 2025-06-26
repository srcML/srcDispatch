// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffElseIfPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_ELSEIF_POLICY_HPP
#define INCLUDED_SRCDIFF_ELSEIF_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicy.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDiffDispatch {

    struct ElseIfData {

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

    class ElseIfPolicy : public ConditionalPolicy<ElseIfData, srcDispatch::ParserState::elseif> {
    public:
        ElseIfPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<ElseIfData, srcDispatch::ParserState::elseif>(listeners) {}
    };

}

#endif
