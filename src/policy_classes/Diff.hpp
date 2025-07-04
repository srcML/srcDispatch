// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Diff.hpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#ifndef INCLUDED_DIFF_HPP
#define INCLUDED_DIFF_HPP

namespace srcDispatch {

enum DiffOperation { COMMON, DELETE, INSERT, CHANGE, NONE };
struct Diff {
    Diff(DiffOperation operation, size_t depth = 0, bool isReplace = false, bool isConvert = false) 
        : operation(operation), depth(depth), isReplace(isReplace), isConvert(isConvert) {}

    DiffOperation operation;
    size_t depth;
    bool isReplace;
    bool isConvert;
};

}

#endif
