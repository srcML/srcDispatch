// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffBlockPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#include <srcDiffClassPolicy.hpp>

namespace srcDiffDispatch {

const std::unordered_map<srcDispatch::ParserState, ClassData::ClassType> ClassPolicy::stateToTypeMapper = {
    {srcDispatch::ParserState::classn,  ClassData::CLASS},
    {srcDispatch::ParserState::structn, ClassData::STRUCT},
};

}
