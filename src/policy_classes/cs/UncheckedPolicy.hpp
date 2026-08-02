// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Unchecked.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_UNCHECKED_POLICY_HPP
#define INCLUDED_UNCHECKED_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <BlockStmtPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class  BlockPolicy;
    struct BlockData;

    struct UncheckedData : public ElementData {

        DeltaElement<std::shared_ptr<BlockData>> block;

        template<class type>
        friend class DeltaElement;
    };

    class UncheckedPolicy : public BlockStmt<UncheckedData, srcDispatch::ParserState::uncheckedstmt> {
    public:
        UncheckedPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : BlockStmt<UncheckedData, srcDispatch::ParserState::uncheckedstmt>(listeners) {}
    };

}

#endif
