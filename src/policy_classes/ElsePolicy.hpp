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
#include <DeltaElement.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ElseData {

        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

        DeltaElement<std::shared_ptr<ConditionData>> condition;
        DeltaElement<std::shared_ptr<BlockData>>     block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            assert(!condition);
            return "";
        }
    };

    class ElsePolicy : public ConditionalPolicy<ElseData, srcDispatch::ParserState::elsestmt> {
    public:
        ElsePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ConditionalPolicy<ElseData, srcDispatch::ParserState::elsestmt>(listeners) {}
    };

}

#endif
