// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Unsafe.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_UNSAFE_POLICY_HPP
#define INCLUDED_UNSAFE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <BlockStmt.hpp>
#include <BlockPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    class BlockPolicy;
    struct BlockData;

    struct UnsafeData : public ElementData {

        DeltaElement<std::shared_ptr<BlockData>>   block;

        template<class type>
        friend class DeltaElement;
    };

    class UnsafePolicy : public BlockStmt<UnsafeData, srcDispatch::ParserState::unsafestmt> {
    public:
        UnsafePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : BlockStmt<UnsafeData, srcDispatch::ParserState::unsafestmt>(listeners) {}
    };

}

#endif
