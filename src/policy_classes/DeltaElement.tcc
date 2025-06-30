// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DeltaElement.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#include <DeltaElement.hpp>

#ifdef __linux__
    #include <execinfo.h>
#endif

#include <iterator>
#include <type_traits>
#include <sstream>

template <class type>
struct Value {
    Value(type& value) : value(value) {}
    type& value;
};

template <class type>
struct Value<std::shared_ptr<type>> {
    Value(std::shared_ptr<type>& value) : value(*value) {}
    type& value;
};

template <class type>
struct ValueConst {
    ValueConst(const type& value) : value(value) {}
    const type& value;
};

template <class type>
struct ValueConst<std::shared_ptr<type>> {
    ValueConst(const std::shared_ptr<type>& value) : value(*value) {}
    const type& value;
};

template <class type>
std::ostream& operator<<(std::ostream& out, const srcDispatch::DeltaElement<type>& element) {
    assert(element.GetOperation() != srcDispatch::NONE);

    return out << element.ToString(srcDispatch::NONE);
}

namespace srcDispatch {

template <class type>
DeltaElement<type>::DeltaElement()
    : original(), modified(), operation(srcDispatch::NONE) {
}

template <class type>
DeltaElement<type>::DeltaElement(const type& element)
    : original(element), modified(), operation(srcDispatch::COMMON) {
}

template <class type>
DeltaElement<type>::DeltaElement(srcDispatch::DiffOperation operation, const type& element)
    : operation(srcDispatch::NONE) {
    Update(operation, element);
}
template <class type>
DeltaElement<type> DeltaElement<type>::copyAs(srcDispatch::DiffOperation operation) const {
    if constexpr (std::is_same_v<type, std::shared_ptr<typename type::element_type>>) {
        return DeltaElement<type>(operation, std::make_shared(this->GetElement()->copyAs()));
    } else {
        return DeltaElement<type>(operation, this->GetElement()->copyAs());
    }
}

template <class type>
bool DeltaElement<type>::IsCommon() const {
    return operation == srcDispatch::COMMON;
}

template <class type>
bool DeltaElement<type>::IsDelete() const {
    return operation == srcDispatch::DELETE;
}

template <class type>
bool DeltaElement<type>::IsInsert() const {
    return operation == srcDispatch::INSERT;
}

template <class type>
bool DeltaElement<type>::IsChange() const {
    return operation == srcDispatch::CHANGE;
}

template <class type>
bool DeltaElement<type>::HasOriginal() const {
    return bool(original);
}

template <class type>
bool DeltaElement<type>::HasModified() const {
    return bool(modified);
}

template <class type>
const std::type_info& DeltaElement<type>::OriginalType() const {
    return original->type();
}

template <class type>
const std::type_info& DeltaElement<type>::ModifiedType() const {
    return modified->type();
}

template <class type>
const type& DeltaElement<type>::GetOriginal() const {
    assert(HasOriginal());
    return *original;
}

template <class type>
type& DeltaElement<type>::GetOriginal() {
    assert(HasOriginal());
    return *original;
}

template <class type>
void DeltaElement<type>::SetOriginal(const type& original) {
    this->original = original;
}

template <class type>
const type& DeltaElement<type>::GetModified() const {
    assert(HasModified());
    return *modified;
}

template <class type>
type& DeltaElement<type>::GetModified() {
    assert(HasModified());
    return *modified;
}

template <class type>
void DeltaElement<type>::SetModified(const type& modified) {
    this->modified = modified;
}

template <class type>
const type& DeltaElement<type>::GetElement() const {
    if(operation == srcDispatch::COMMON) return *original;
    if(operation == srcDispatch::DELETE) return *original;

    return *modified;
}

template <class type>
type& DeltaElement<type>::GetElement() {
    if(operation == srcDispatch::COMMON) return *original;
    if(operation == srcDispatch::DELETE) return *original;

    return *modified;
}

template <class type>
const auto DeltaElement<type>::operator->() const {
    if constexpr (std::is_same_v<type, std::shared_ptr<typename type::element_type>>) {
        return &*GetElement();
    } else {
        return &GetElement();    
    }
}

template <class type>
auto DeltaElement<type>::operator->() {
    if constexpr (std::is_same_v<type, std::shared_ptr<typename type::element_type>> ) {
        return &*GetElement();
    } else {
        return &GetElement();    
    }
}

template <class type>
DeltaElement<type>::operator bool() const {
    return operation != srcDispatch::NONE;
}

template <class type>
bool DeltaElement<type>::operator==(const DeltaElement<type>& that) {
    if(this->operation != that.operation)                      return false;
    if(this->HasOriginal() != that.HasOriginal())              return false;
    if(this->HasModified() != that.HasModified())              return false;
    if(this->HasOriginal() && this->original != that.original) return false;
    if(this->HasModified() && this->modified != that.modified) return false;

    return true;    
}

template <class type>
bool DeltaElement<type>::operator==(const type& that) {
    if(!*this) return false;

    return GetElement() == that;
}



template <class type>
srcDispatch::DiffOperation DeltaElement<type>::GetOperation() const {
    return operation;
}

template <class type>
void DeltaElement<type>::SetOperation(enum srcDispatch::DiffOperation operation) {
    this->operation = operation;
}

template <class type>
bool DeltaElement<type>::IsOfOperation(srcDispatch::DiffOperation operation) const {
    if(this->operation == srcDispatch::NONE)   return false;
    if(this->operation == srcDispatch::COMMON) return true;
    if(this->operation == srcDispatch::CHANGE) return true;

    return this->operation == operation;
}

template <class type>
const type& DeltaElement<type>::GetOfOperation(srcDispatch::DiffOperation operation) const {
    assert(IsOfOperation(operation));

    if(this->operation == srcDispatch::COMMON) return *original;
    if(this->operation == srcDispatch::CHANGE) return operation == srcDispatch::DELETE? *original : *modified;
    if(operation == srcDispatch::DELETE) return *original;
    return *modified;
}

template <class type>
void DeltaElement<type>::Update(srcDispatch::DiffOperation operation, const type& element) {

    if(    (this->operation == srcDispatch::DELETE && operation == srcDispatch::INSERT)
        || (this->operation == srcDispatch::INSERT && operation == srcDispatch::DELETE)) {
        this->operation = srcDispatch::CHANGE;
    } else if(this->operation != srcDispatch::CHANGE) {
        this->operation = operation;
    }

    if(operation == srcDispatch::COMMON) {
        original = element;
    } else if(operation == srcDispatch::DELETE) {
        original = element;
    } else if(operation == srcDispatch::INSERT) {
        modified = element;
    } else if(operation == srcDispatch::CHANGE) {
        original = element;
        modified = element;
    }

}

template <class type>
template <class any_type>
void DeltaElement<type>::Append(srcDispatch::DiffOperation operation, const std::string& str) {

    if(this->operation == srcDispatch::DELETE && operation == srcDispatch::INSERT) {
        this->operation = srcDispatch::CHANGE;
    } else if(this->operation != srcDispatch::CHANGE) {
        this->operation = operation;
    }

    if constexpr (!std::is_same_v<type, any_type>) {
        static const any_type null_any_type;
        any_type* innerOriginal = original? std::any_cast<any_type>(&*original) : nullptr;
        any_type* innerModified = modified? std::any_cast<any_type>(&*modified) : nullptr;

        if(operation == srcDispatch::COMMON) {
            Value(*innerOriginal).value += str;
        } else if(operation == srcDispatch::DELETE) {
            Value(*innerOriginal).value += str;
        } else if(operation == srcDispatch::INSERT) {
            Value(*innerModified).value += str;
        } else if(operation == srcDispatch::CHANGE) {
            Value(*innerOriginal).value += str;
            Value(*innerModified).value += str;
        }

    } else {

        if(operation == srcDispatch::COMMON) {
            Value(original).value += str;
        } else if(operation == srcDispatch::DELETE) {
            Value(original).value += str;
        } else if(operation == srcDispatch::INSERT) {
            Value(modified).value += str;
        } else if(operation == srcDispatch::CHANGE) {
            Value(original).value += str;
            Value(modified).value += str;
        }

    }
    
}

template <class type>
void DeltaElement<type>::Clear() {
    original = std::optional<type>();
    modified = std::optional<type>();
}

template <class type>
template <class any_type>
std::string DeltaElement<type>::ToString(srcDispatch::DiffOperation operation) const {

    if(this->operation == srcDispatch::NONE) return "";

    std::string originalStr;
    std::string modifiedStr;

    if constexpr (!std::is_same_v<type, any_type>) {

        DeltaElement<any_type> innerElement(this->operation);
        if(original) {
            innerElement.GetOriginal() = std::any_cast<any_type>(*original);
        }

        if(modified) {
            innerElement.GetModified() = std::any_cast<any_type>(*modified);
        }

        return innerElement.ToString(operation);
    } else {

        if constexpr (std::is_same_v<type, std::string> || std::is_same_v<type, std::shared_ptr<std::string>>) {
            if(HasOriginal()) {
                originalStr = ValueConst(*original).value;
            }
        } else {
            const type& element = HasOriginal()? *original : *modified;
            originalStr = ValueConst(*element).value.ToString(srcDispatch::DELETE);
        }

        if constexpr (std::is_same_v<type, std::string> || std::is_same_v<type, std::shared_ptr<std::string>>) {
            if(HasModified() || this->operation == srcDispatch::COMMON) {
                const type& element = this->operation == srcDispatch::COMMON? *original : *modified;
                modifiedStr = ValueConst(element).value;
            }
        } else {
            const type& element = HasModified()? *modified : *original;
            modifiedStr = ValueConst(element).value.ToString(srcDispatch::INSERT);
        }

        if(operation == srcDispatch::DELETE) {
            return originalStr;
        } else if(operation == srcDispatch::INSERT) {
            return modifiedStr;
        } else if(originalStr != modifiedStr) {
            return originalStr + '|' + modifiedStr;
        }
    }

    return originalStr;
}

}
