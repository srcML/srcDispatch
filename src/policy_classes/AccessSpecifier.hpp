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
#include <mutex>

namespace srcDispatch {

class AccessSpecifier {
public:
    virtual ~AccessSpecifier() {}
    virtual std::string ToString() const = 0;
protected:
    AccessSpecifier() {}
};

class PublicAccess : public AccessSpecifier {
public:
    static PublicAccess& Get() {
        static PublicAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "public"; };
private:
    PublicAccess() {}
};

class PrivateAccess : public AccessSpecifier {
public:
    static PrivateAccess& Get() {
        static PrivateAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "private"; };
private:
    PrivateAccess() {}
};

class ProtectedAccess : public AccessSpecifier {
public:
    static ProtectedAccess& Get() {
        static ProtectedAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "protected"; };
private:
    ProtectedAccess() {}
};

class NullAccess : public AccessSpecifier {
public:
    static NullAccess& Get() {
        static NullAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "null"; };
private:
    NullAccess() {}
};

void NullDeleter(AccessSpecifier*);

const std::shared_ptr<AccessSpecifier> PUBLIC_ACCESS    = std::shared_ptr<AccessSpecifier>(static_cast<AccessSpecifier*>(&PublicAccess::Get()),    NullDeleter);
const std::shared_ptr<AccessSpecifier> PRIVATE_ACCESS   = std::shared_ptr<AccessSpecifier>(static_cast<AccessSpecifier*>(&PrivateAccess::Get()),   NullDeleter);
const std::shared_ptr<AccessSpecifier> PROTECTED_ACCESS = std::shared_ptr<AccessSpecifier>(static_cast<AccessSpecifier*>(&ProtectedAccess::Get()), NullDeleter);
const std::shared_ptr<AccessSpecifier> NULL_ACCESS      = std::shared_ptr<AccessSpecifier>(static_cast<AccessSpecifier*>(&NullAccess::Get()),      NullDeleter);

std::shared_ptr<AccessSpecifier> AccessSpecifierFactory(const std::string& str);

}

#endif
