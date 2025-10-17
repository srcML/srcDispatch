// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Access.cpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#include <Access.hpp>

#include <unordered_map>

namespace srcDispatch {

void NullDeleter(Access*) {}

std::shared_ptr<Access> AccessFactory(const std::string& str) {
    static std::unordered_map<std::string, std::shared_ptr<Access>> accessSpecifierMap
     = {
        {"public",    PUBLIC_ACCESS    },
        {"private",   PRIVATE_ACCESS   },
        {"protected", PROTECTED_ACCESS },
       };

       return accessSpecifierMap.contains(str)? accessSpecifierMap.at(str) : NULL_ACCESS;
}

}
