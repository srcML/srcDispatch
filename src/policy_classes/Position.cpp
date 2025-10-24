// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Position.cpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#include <Position.hpp>

#include <cassert>

namespace srcDispatch {

Position::Position() : line(0), column(0) {
}

Position::Position(const std::string& posStr) : Position() {

   int colonPos = posStr.find(':');
   if(colonPos == std::string::npos) return;

   line   = std::stoi(posStr.substr(0, colonPos));
   column = std::stoi(posStr.substr(colonPos + 1));

}

bool Position::operator==(const Position& that) {
   return line == that.line && column == that.column;

}

std::string Position::ToString() const {
   return std::to_string(line) + ':' + std::to_string(column);
}

}
