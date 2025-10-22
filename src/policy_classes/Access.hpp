// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Access.hpp
 *
 * @copyright Copyright (C) 2025-2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Infrastructure.
 */

#ifndef INCLUDED_ACCESS_HPP
#define INCLUDED_ACCESS_HPP

#include <string>
#include <memory>
#include <mutex>

namespace srcDispatch {

class Access {
public:
    Access(const Access&) = delete;
    const Access& operator=(Access) = delete;
    virtual ~Access() {}
    virtual std::string ToString() const = 0;
protected:
    Access() {}
};

class PublicAccess : public Access {
public:
    static PublicAccess& Get() {
        static PublicAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "public"; };
private:
    PublicAccess() {}
};

class PrivateAccess : public Access {
public:
    static PrivateAccess& Get() {
        static PrivateAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "private"; };
private:
    PrivateAccess() {}
};

class ProtectedAccess : public Access {
public:
    static ProtectedAccess& Get() {
        static ProtectedAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "protected"; };
private:
    ProtectedAccess() {}
};

class NullAccess : public Access {
public:
    static NullAccess& Get() {
        static NullAccess access;
        return access; 
    }
    virtual std::string ToString() const { return "null"; };
private:
    NullAccess() {}
};

void NullDeleter(Access*);

const std::shared_ptr<Access> PUBLIC_ACCESS    = std::shared_ptr<Access>(static_cast<Access*>(&PublicAccess::Get()),    NullDeleter);
const std::shared_ptr<Access> PRIVATE_ACCESS   = std::shared_ptr<Access>(static_cast<Access*>(&PrivateAccess::Get()),   NullDeleter);
const std::shared_ptr<Access> PROTECTED_ACCESS = std::shared_ptr<Access>(static_cast<Access*>(&ProtectedAccess::Get()), NullDeleter);
const std::shared_ptr<Access> NULL_ACCESS      = std::shared_ptr<Access>(static_cast<Access*>(&NullAccess::Get()),      NullDeleter);

std::shared_ptr<Access> AccessFactory(const std::string& str);

}

#endif
