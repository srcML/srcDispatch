// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file SyncStmtPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SYNCSTMT_POLICY_HPP
#define INCLUDED_SYNCSTMT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <InitStmtPolicy.hpp>
#include <InitPolicy.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class  BlockPolicy;
    struct BlockData;

    class  InitPolicy;
    struct InitData;

    struct SyncStmtData : public ElementData {

        DeltaElement<std::shared_ptr<BlockData>> block;
        DeltaElement<std::shared_ptr<InitData>>  init;

        template<class type>
        friend class DeltaElement;
    };

    class SyncStmtPolicy : public InitStmt<SyncStmtData, srcDispatch::ParserState::syncstmt> {
    public:
        SyncStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : InitStmt<SyncStmtData, srcDispatch::ParserState::syncstmt>(listeners) {}
    };

}

#endif
