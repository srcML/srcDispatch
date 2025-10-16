// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file AccessSpecifier.cpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#include <AccessSpecifier.hpp>

#include <unordered_map>

namespace srcDispatch {

const std::shared_ptr<AccessSpecifier> PUBLIC_ACCESS    = std::make_shared<PublicAccess>();
const std::shared_ptr<AccessSpecifier> PRIVATE_ACCESS   = std::make_shared<PrivateAccess>();
const std::shared_ptr<AccessSpecifier> PROTECTED_ACCESS = std::make_shared<ProtectedAccess>();
const std::shared_ptr<AccessSpecifier> NULL_ACCESS      = std::make_shared<NullAccess>();

std::shared_ptr<AccessSpecifier> AccessSpecifierFactory(const std::string& str) {
        static std::unordered_map<std::string, std::shared_ptr<AccessSpecifier>> accessSpecifierMap
         = {
            {"public",    PUBLIC_ACCESS    },
            {"private",   PRIVATE_ACCESS   },
            {"protected", PROTECTED_ACCESS },
           };

           return accessSpecifierMap.contains(str)? accessSpecifierMap.at(str) : NULL_ACCESS;
    }

}
