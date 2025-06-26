// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file IfPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_IF_POLICY_HPP
#define INCLUDED_SRCDIFF_IF_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicy.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct IfData {

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

    class IfPolicy : public ConditionalPolicy<IfData, srcDispatch::ParserState::ifstmt> {
    public:
        IfPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<IfData, srcDispatch::ParserState::ifstmt>(listeners) {}
    };

}

#endif
