/**
 * @file ParamTypePolicy.hpp
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
#ifndef INCLUDED_PARAM_TYPE_POLICY_HPP
#define INCLUDED_PARAM_TYPE_POLICY_HPP

#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <DeclDS.hpp>
class ParamTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ParamTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {}
    protected:
        void * DataInner() const override {
            return new DeclData(data);
        }
    private:
        DeclData data;
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::type, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    data.namespaces.push_back(ctx.currentToken);
                }
            };
            openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx){
                data.usesSubscript = true;
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::parameter)){
                    if(currentModifier == "*"){
                        data.isPointer = true;
                    }
                    else if(currentModifier == "&"){
                        data.isReference = true;
                    }
                }
            };

            closeEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::parameter})){
                    data.linenumber = ctx.currentLineNumber;
                    data.nameOfIdentifier = currentDeclName;
                }
            };

            closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::parameter})){
                    data.nameOfType = currentTypeName;
                    currentTypeName.clear();
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken[0] == ' ')){
                    if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                        currentTypeName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::decl, ParserState::parameter}) && 
                       ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, 
                        ParserState::init, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                        currentDeclName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::specifier, ParserState::decl, ParserState::parameter})){
                        currentSpecifier = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::modifier, ParserState::type, ParserState::parameter})){
                        currentModifier = ctx.currentToken;
                    }
                }
            };
            closeEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx){
                data.isParameter = true;
                NotifyAll(ctx);
                data.clear();
            };
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::parameter)){
                    if(currentSpecifier == "static"){
                        data.isStatic = true;
                    }
                    if(currentSpecifier == "const"){
                        if(data.isPointer){
                            data.isConstAlias = true;
                        }else{
                            data.isConstValue = true;
                        }
                    }
                }
                currentSpecifier.clear();
            };

        }
};

#endif

