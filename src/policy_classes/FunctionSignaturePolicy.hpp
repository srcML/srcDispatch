/**
 * @file FunctionSignaturePolicy.hpp
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
#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <vector>
#include <ParamTypePolicy.hpp>
#include <DeclDS.hpp>
#ifndef FUNCTIONSIGNATUREPOLICY
#define FUNCTIONSIGNATUREPOLICY
struct SignatureData{
    SignatureData():isConst{false}, constPointerReturn{false}, isMethod{false}, isStatic{false}, pointerToConstReturn{false}, 
    hasAliasedReturn{false}, hasSideEffect{false}{}
    int linenumber;
    bool isConst;
    bool isMethod;
    bool isStatic;
    std::string name;
    bool hasSideEffect;
    bool hasAliasedReturn;
    std::string returnType;
    bool constPointerReturn;
    bool pointerToConstReturn;
    std::string sLexicalCategory;
    std::string returnTypeModifier;
    std::vector<DeclData> parameters;
    std::string nameOfContainingFile;
    std::string nameOfContainingClass;
    std::vector<std::string> functionNamespaces;
    std::vector<std::string> returnTypeNamespaces;
    void clear(){
        name.clear();
        isConst = false;
        isMethod = false;
        isStatic = false;
        returnType.clear();
        parameters.clear();
        hasSideEffect = false;
        sLexicalCategory.clear();
        hasAliasedReturn = false;
        functionNamespaces.clear();
        returnTypeModifier.clear();
        constPointerReturn = false;
        nameOfContainingFile.clear();
        returnTypeNamespaces.clear();
        pointerToConstReturn = false;
        nameOfContainingClass.clear();
    }
};
class FunctionSignaturePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~FunctionSignaturePolicy(){}
        FunctionSignaturePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            currentArgPosition = 1;
            parampolicy.AddListener(this);
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            paramdata = policy->Data<DeclData>();
            data.parameters.push_back(*paramdata);
        }
    protected:
        void * DataInner() const override {
            return new SignatureData(data);
        }
    private:
        bool seenModifier;
        ParamTypePolicy parampolicy;
        DeclData* paramdata;
        SignatureData data;
        size_t currentArgPosition;       
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                data.linenumber = ctx.currentLineNumber;
                ctx.dispatcher->AddListener(&parampolicy);
            };
            openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::type, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::functionblock, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    data.returnTypeNamespaces.push_back(ctx.currentToken);
                }
                if(ctx.IsOpen(ParserState::function) && ctx.Nor({ParserState::type, ParserState::parameterlist, ParserState::functionblock, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    data.functionNamespaces.push_back(ctx.currentToken);
                }
            };
            openEventMap[ParserState::functionblock] = [this](srcSAXEventContext& ctx){//incomplete. Blocks count too.
                if(ctx.IsOpen(ParserState::classn)){
                    data.isMethod = true;
                    data.nameOfContainingClass = ctx.currentClassName;
                }
                data.name = ctx.currentFunctionName;
                NotifyAll(ctx);
                seenModifier = false;
                data.clear();
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
                if(currentModifier == "*") {
                    if(ctx.And({ParserState::type, ParserState::function}) && ctx.IsClosed(ParserState::parameterlist)){
                        seenModifier = true;
                        data.hasAliasedReturn = true;
                    }
                }
                else if(currentModifier == "&") {}
            };
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::name, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::parameterlist, ParserState::genericargumentlist})){
                    data.returnType = ctx.currentToken;
                }
                if(ctx.And({ParserState::modifier, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::genericargumentlist})){
                    data.returnTypeModifier = ctx.currentToken;
                }
                if(ctx.And({ParserState::specifier, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::functionblock, ParserState::genericargumentlist})){
                    currentSpecifier = ctx.currentToken;
                }
                if(ctx.And({ParserState::modifier, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::functionblock, ParserState::genericargumentlist})){
                    currentModifier = ctx.currentToken;
                }
            };
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
                if(currentSpecifier == "const" && ctx.Nor({ParserState::parameterlist, ParserState::type})){
                    data.isConst = true;
                }
                if(currentSpecifier == "const" && ctx.IsOpen(ParserState::function) && ctx.IsOpen(ParserState::type)){
                    if(!seenModifier){
                        data.pointerToConstReturn = true;
                    }else{
                        data.constPointerReturn = true;
                    }
                }
                if(currentSpecifier == "static" && ctx.Nor({ParserState::parameterlist, ParserState::type})){
                    data.isStatic = true;
                }
                currentSpecifier.clear();
            };
            closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->RemoveListener(&parampolicy);
            };
        }

};
#endif