// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DeltaElement.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_DELTA_ELEMENT_HPP
#define INCLUDED_DELTA_ELEMENT_HPP

#include <srcDispatchUtilities.hpp>

#include <iostream>
#include <optional>
#include <cassert>

namespace srcDispatch {

template<class type>
class DeltaElement {

private:
    std::optional<type> original;
    std::optional<type> modified;

    srcDispatch::DiffOperation operation;

protected:

public:
    DeltaElement();
    DeltaElement(const type& element);
    DeltaElement(srcDispatch::DiffOperation operation, const type& element = type());

    DeltaElement copyAs(srcDispatch::DiffOperation operation) const;

    bool IsCommon() const;
    bool IsDelete() const;
    bool IsInsert() const;
    bool IsChange() const;

    bool HasOriginal() const;
    bool HasModified() const;

    const std::type_info& OriginalType() const;
    const std::type_info& ModifiedType() const;

    const type& GetOriginal() const;
    type& GetOriginal();
    void SetOriginal(const type& original);

    const type& GetModified() const;
    type& GetModified();
    void SetModified(const type& modified);

    const type& GetElement() const;
    type& GetElement();

    const auto operator->() const;
    auto operator->();

    operator bool() const;
    bool operator==(const DeltaElement& that);
    bool operator==(const type& that);

    srcDispatch::DiffOperation GetOperation() const;
    void SetOperation(srcDispatch::DiffOperation operation);

    bool IsOfOperation(srcDispatch::DiffOperation operation) const;
    const type& GetOfOperation(srcDispatch::DiffOperation operation) const;

    void Update(srcDispatch::DiffOperation operation, const type& element);

    template<class any_type = type>
    void Append(srcDispatch::DiffOperation operation, const std::string& str);
    void Clear();

    template <class any_type = type>
    std::string ToString(srcDispatch::DiffOperation operation = srcDispatch::NONE) const;
};

}

#include <DeltaElement.tcc>

#endif
