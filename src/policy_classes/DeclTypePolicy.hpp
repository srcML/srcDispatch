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
#ifndef INCLUDED_DECL_TYPE_POLICY_HPP
#define INCLUDED_DECL_TYPE_POLICY_HPP

#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <DeclDS.hpp>
class DeclTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        DeclData data;
        ~DeclTypePolicy(){}
        DeclTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

        void Finalize(srcSAXEventDispatch::srcSAXEventContext& ctx){  
            using namespace srcSAXEventDispatch;

                if( ctx.And({ParserState::declstmt}) || ctx.And({ParserState::forstmt, ParserState::control, ParserState::init}) || 
                    ctx.And({ParserState::switchstmt, ParserState::condition}) ){
                    if(ctx.IsOpen(ParserState::classblock) && ctx.Nor({ParserState::function, ParserState::constructor, ParserState::destructor})){
                        data.isClassMember = true;
                        data.nameOfContainingClass = ctx.currentClassName;
                    }

                    data.nameOfContainingFile = ctx.currentFilePath;

                    if(ctx.currentFunctionName.empty() && !ctx.currentClassName.empty()){
                        data.nameOfContainingClass = ctx.currentClassName;
                    }

                    data.nameOfContainingFunction = ctx.currentFunctionName;

                    data.lineNumber = ctx.currentLineNumber;
                    data.nameOfIdentifier = currentDeclName;


                    data.isLocalVar = true;
                    if(ctx.currentFileLanguage == "Java" && !data.isFinal){
                        data.isReference = true;
                    }

                    currentDeclName.clear();
                    NotifyAll(ctx);
                    data.clear();
                }

                if(ctx.And({ParserState::parameter})){
                    data.lineNumber = ctx.currentLineNumber;
                    data.nameOfIdentifier = currentDeclName;
                    
                    // Used to mark line numbers where parameters are declared
                    if (possibleDefs.size() == 0) {
                        possibleDefs.push_back(ctx.currentLineNumber);
                    } else {
                        if (possibleDefs.back() != ctx.currentLineNumber) {
                            possibleDefs.push_back(ctx.currentLineNumber);
                        }
                    }

                    data.isParameter = true;
                    data.nameOfContainingFunction = ctx.currentFunctionName;
                    data.nameOfContainingFile = ctx.currentFilePath;
                    if (ctx.currentFileLanguage == "Java" && !data.isFinal){
                        data.isReference = true;
                    }
                    NotifyAll(ctx);
                    data.clear();
                }
        }
        
        std::vector<unsigned int>* GetPossibleDefs() {
            return &possibleDefs;
        }
        std::vector<std::string>* GetParamNames() {
            return &paramNames;
        }
    protected:
        void * DataInner() const override { 
            return new DeclData(data);
        }
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        std::vector<unsigned int> possibleDefs;
        std::vector<std::string> paramNames;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){ 
                bool isDeclToken = ( ctx.And({ParserState::type, ParserState::declstmt}) &&
                                    ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist}) ||
                                    ctx.IsOpen({ParserState::init}) );
                bool isParamToken = ( ctx.And({ParserState::type, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist}) );

                if(isDeclToken || isParamToken){
                    data.namespaces.push_back(ctx.currentToken);
                }
            };

            openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx){ 
                data.usesSubscript = true;
            };

            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){ 
                if( ctx.IsOpen(ParserState::declstmt) || ctx.IsOpen({ParserState::init}) || ctx.IsOpen(ParserState::parameter) ){
                    if(currentModifier == "*"){
                        data.isPointer = true;
                    }
                    if(currentModifier == "&"){
                        data.isReference = true;
                    }
                }
            };

            closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx){ 
                if( ctx.And({ParserState::declstmt}) || ctx.IsOpen({ParserState::init}) ){
                    data.nameOfType = currentTypeName;
                }

                if(ctx.And({ParserState::parameter})){
                    data.nameOfType = currentTypeName;
                    currentTypeName.clear();
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){ 
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken[0] == ' ')){
                    if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::declstmt}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist, ParserState::index})){
                        currentTypeName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist, ParserState::index})){
                        currentTypeName = ctx.currentToken; // parameter data type
                    }
                    if( ctx.IsOpen(ParserState::forstmt) && ctx.And({ParserState::name, ParserState::type, ParserState::decl}) && 
                        ctx.Nor({ParserState::index, ParserState::argumentlist, ParserState::specifier, ParserState::modifier}) ) {
                        currentTypeName = ctx.currentToken;
                    }
                    if( ctx.IsOpen(ParserState::switchstmt) && ctx.And({ParserState::name, ParserState::type, ParserState::decl}) && 
                        ctx.Nor({ParserState::index, ParserState::argumentlist, ParserState::specifier, ParserState::modifier}) ) {
                        currentTypeName = ctx.currentToken;
                    }


                    if( ctx.And({ParserState::name, ParserState::decl, ParserState::declstmt}) &&
                       ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, ParserState::init, ParserState::specifier, ParserState::modifier}) ){
                        currentDeclName = ctx.currentToken;
                    }
                    if( ctx.IsOpen(ParserState::forstmt) && ctx.And({ParserState::name, ParserState::decl}) && 
                        ctx.Nor({ParserState::type, ParserState::index, ParserState::argumentlist, ParserState::specifier, ParserState::modifier, ParserState::expr}) ) {
                        currentDeclName = ctx.currentToken;
                    }
                    if( ctx.IsOpen(ParserState::switchstmt) && ctx.And({ParserState::name, ParserState::decl}) && 
                        ctx.Nor({ParserState::type, ParserState::index, ParserState::argumentlist, ParserState::specifier, ParserState::modifier, ParserState::expr}) ) {
                        currentDeclName = ctx.currentToken;
                    }

                    if(ctx.And({ParserState::name, ParserState::decl, ParserState::parameter}) && 
                        ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, 
                        ParserState::init, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                        currentDeclName = ctx.currentToken; // parameter variable name

                        if (paramNames.size() == 0) {
                            paramNames.push_back(ctx.currentToken);
                        } else {
                            if (paramNames.back() != ctx.currentToken) {
                                paramNames.push_back(ctx.currentToken);
                            }
                        }
                    }

                    if( ctx.And({ParserState::specifier, ParserState::decl, ParserState::declstmt}) || ctx.And({ParserState::specifier, ParserState::decl, ParserState::parameter}) ){
                        currentSpecifier = ctx.currentToken;
                    }
                    
                    if( ctx.And({ParserState::modifier, ParserState::type, ParserState::declstmt}) || ctx.And({ParserState::modifier, ParserState::type, ParserState::parameter}) ){
                        currentModifier = ctx.currentToken;
                    }
                }
            };
            
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx){ 
                if( ctx.IsOpen(ParserState::declstmt) || ctx.IsOpen({ParserState::init}) || ctx.IsOpen(ParserState::parameter) ){
                    if(currentSpecifier == "const"){
                        if(data.isPointer){
                            data.isConstAlias = true;
                        }else{
                            data.isConstValue = true;
                        }
                    }
                    if(currentSpecifier == "final"){
                        data.isFinal = true;
                    }
                    if(currentSpecifier == "static"){
                        data.isStatic = true;
                    }
                }

                currentSpecifier.clear();
            };

        }
};

#endif