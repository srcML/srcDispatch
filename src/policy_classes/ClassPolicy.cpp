// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file BlockPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#include <ClassPolicy.hpp>

namespace srcDispatch {

const std::unordered_map<srcDispatch::ParserState, ClassData::ClassType> ClassPolicy::stateToTypeMapper = {
    {srcDispatch::ParserState::classn,     ClassData::CLASS},
    {srcDispatch::ParserState::classdecl,  ClassData::CLASS_DECL},
    {srcDispatch::ParserState::structn,    ClassData::STRUCT},
    {srcDispatch::ParserState::structdecl, ClassData::STRUCT_DECL},
    {srcDispatch::ParserState::enumdecl,   ClassData::ENUM_DECL},
    {srcDispatch::ParserState::uniondecl,  ClassData::UNION_DECL},
};

}
