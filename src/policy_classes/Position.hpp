// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Position.hpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#ifndef INCLUDED_POSITION_HPP
#define INCLUDED_POSITION_HPP

#include <string>

namespace srcDispatch {

class Position {
public:
    Position();
    Position(const std::string& posStr);

    bool operator==(const Position& that);
    std::string ToString() const;
private:
    std::size_t line;
    std::size_t column;
};

}

#endif
