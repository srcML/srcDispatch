// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file AccessSpecifier.hpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#ifndef INCLUDED_ACCESS_SPECIFIER_HPP
#define INCLUDED_ACCESS_SPECIFIER_HPP

#include <string>
#include <srcDispatchUtilities.hpp>

enum AccessSpecifier {
    NONE      = 0,
    PUBLIC    = 1,
    PRIVATE   = 2,
    PROTECTED = 3 
};

namespace srcDispatch {
    // might be better to use an unordered_map, but would need a custom hash since the key is std::pair<ParserState, std::string>
    inline AccessSpecifier getDefaultAccessSpecifier(ParserState pstate, std::string language) {
        switch (pstate) {
            case ParserState::classn:
                if (language == "C#" || language == "Java" || language == "C++") 
                    return AccessSpecifier::PRIVATE;
                else
                    return AccessSpecifier::NONE;
                break;
            case ParserState::structn:
                if (language == "C#")
                    return AccessSpecifier::PRIVATE;
                else if (language == "C++")
                    return AccessSpecifier::PUBLIC;
                else
                    return AccessSpecifier::NONE;
                break;
            // Uncomment when interfaces (issue 38) are added
            // case ParserState::interfacen:
            //     if (language == "Java" || language == "C#")
            //         return AccessSpecifier::PUBLIC;
            //     else
            //         return AccessSpecifier::NONE;
            //     break;
            default:
                return AccessSpecifier::NONE;

        }
    }
}

#endif
