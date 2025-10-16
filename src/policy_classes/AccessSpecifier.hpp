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
#include <memory>

namespace srcDispatch {

class AccessSpecifier {
public:
    AccessSpecifier() {}
    virtual ~AccessSpecifier() {}
    virtual std::string ToString() const = 0;
};

class PublicAccess : public AccessSpecifier {
public:
    PublicAccess() {}
    virtual std::string ToString() const { return "public"; };
};

class PrivateAccess : public AccessSpecifier {
public:
    PrivateAccess() {}
    virtual std::string ToString() const { return "private"; };
};

class ProtectedAccess : public AccessSpecifier {
public:
    ProtectedAccess() {}
    virtual std::string ToString() const { return "protected"; };
};

class NullAccess : public AccessSpecifier {
public:
    NullAccess() {}
    virtual std::string ToString() const { return "null"; };
};

extern const std::shared_ptr<AccessSpecifier> PUBLIC_ACCESS;
extern const std::shared_ptr<AccessSpecifier> PRIVATE_ACCESS;
extern const std::shared_ptr<AccessSpecifier> PROTECTED_ACCESS;
extern const std::shared_ptr<AccessSpecifier> NULL_ACCESS;

std::shared_ptr<AccessSpecifier> AccessSpecifierFactory(const std::string& str);

}

#endif
