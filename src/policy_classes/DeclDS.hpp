/**
 * @file DeclTypePolicy.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef INCLUDED_DECL_DS_HPP
#define INCLUDED_DECL_DS_HPP

struct DeclData{
    DeclData(): linenumber{0}, isConst{false}, isConstAlias{false}, isAliasToConst{false}, isReference{false}, isPointer{false}, isStatic{false}, isClassMember{false}, usesSubscript{false} {}
    void clear(){
        nameoftype.clear();
        nameofidentifier.clear();
        namespaces.clear();
        linenumber = -1;
        isConst = false;
        isReference = false;
        isPointer = false;
        isStatic = false;
        isClassMember = false;
        usesSubscript = false;
    }
    std::string nameoftype;
    std::string nameofidentifier;
    std::vector<std::string> namespaces;
    int linenumber;
    bool isConst;
    bool isConstAlias;
    bool isAliasToConst;
    bool isReference;
    bool isPointer;
    bool isStatic;
    bool isClassMember;
    bool usesSubscript;
};

#endif