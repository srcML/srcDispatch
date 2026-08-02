// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ForeachPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_FOREACH_POLICY_HPP
#define INCLUDED_FOREACH_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ForLikePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class BlockPolicy;
    struct BlockData;

    struct ForeachData : public ElementData {

        DeltaElement<std::shared_ptr<ControlData>> control;
        DeltaElement<std::shared_ptr<BlockData>>   block;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return control.ToString(operation);
        }
    };

    class ForeachPolicy : public ForLike<ForeachData, srcDispatch::ParserState::foreachstmt> {
    public:
        ForeachPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : ForLike<ForeachData, srcDispatch::ParserState::foreachstmt>(listeners) {}
    };

}

#endif
