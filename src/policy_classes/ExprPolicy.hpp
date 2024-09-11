/**
 * @file ExprPolicy.hpp
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
#include <exception>
#include <set>
#include <vector>
#ifndef EXPRPOLICY
#define EXPRPOLICY
class ExprPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        struct ExprData{
            ExprData() {lhs = false;}
            void clear(){
               definitions.clear();
               uses.clear();
               lhs = false;
            }
            bool lhs;
            std::string nameOfIdentifier;
            std::set<unsigned int> definitions;
            std::set<unsigned int> uses; //could be used multiple times in same expr
        };
        struct ExprDataSet{
           ExprDataSet() = default;
           ExprDataSet(std::map<std::string, ExprData> dat){
            dataSet = dat;
           }
           void clear(){
            dataSet.clear();
           }
           std::string lhsName;
           std::map<std::string, ExprData> dataSet;
        };
        ~ExprPolicy(){}
        ExprPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy [[maybe_unused]], const srcSAXEventDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} //doesn't use other parsers
        void NotifyWrite(const PolicyDispatcher * policy [[maybe_unused]], srcSAXEventDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {
            return new ExprDataSet(exprDataSet);
        }
    private:
        ExprData data;
        ExprDataSet exprDataSet;
        std::string currentTypeName, currentExprName, currentModifier, currentSpecifier;
        std::pair<std::string, unsigned int> currentExprOp;
        std::vector<unsigned int> currentLine;
        
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            
            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                // Long or-statement allows various declaration operators to get planted into
                // a slices def data output
                // if (ctx.currentToken == "=" || ctx.currentToken == "++" || ctx.currentToken == "+=" ||
                //     ctx.currentToken == "--" || ctx.currentToken == "-=" || ctx.currentToken == "*=" ||
                //     ctx.currentToken == "/=" || ctx.currentToken == "%=" || ctx.currentToken == "&=" ||
                //     ctx.currentToken == "|=" || ctx.currentToken == "^=" || ctx.currentToken == "<<=" ||
                //     ctx.currentToken == ">>=")

                if (ctx.currentToken == "=" || ctx.currentToken == "+=" ||
                    ctx.currentToken == "-=" || ctx.currentToken == "*=" ||
                    ctx.currentToken == "/=" || ctx.currentToken == "%=" ||
                    ctx.currentToken == "--" || ctx.currentToken == "++"){
                    currentExprOp = std::make_pair(ctx.currentToken, ctx.currentLineNumber);
                    auto it = exprDataSet.dataSet.find(currentExprName);

                    if(it != exprDataSet.dataSet.end()){
                        exprDataSet.lhsName = currentExprName;
                        it->second.lhs = true;
                        it->second.uses.erase(currentLine.back());
                        it->second.definitions.insert(currentLine.back());
                    }else{
                        std::cerr<<"No such thing as: "<<currentExprName<<std::endl;
                    }
                }
            };

            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx){

                if(currentLine.empty() || currentLine.back() != ctx.currentLineNumber){
                    currentLine.push_back(ctx.currentLineNumber);
                }
                
                if(ctx.IsOpen({ParserState::exprstmt})){
                    auto it = exprDataSet.dataSet.find(currentExprName);
                    if(it != exprDataSet.dataSet.end()){
                        it->second.uses.insert(currentLine.back()); //assume it's a use

                        if ( (currentExprOp.first == "++" || currentExprOp.first == "--") && currentExprOp.second == ctx.currentLineNumber )
                            it->second.definitions.insert(currentLine.back());
                    }else{
                        data.nameOfIdentifier = currentExprName;

                        data.uses.insert(currentLine.back());

                        if ( (currentExprOp.first == "++" || currentExprOp.first == "--") && currentExprOp.second == ctx.currentLineNumber )
                            data.definitions.insert(currentLine.back());

                        exprDataSet.dataSet.insert(std::make_pair(currentExprName, data));
                    }
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken == " ")){
                    if(ctx.And({ParserState::name, ParserState::expr, ParserState::exprstmt}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::op})){
                        currentExprName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::specifier, ParserState::expr, ParserState::exprstmt})){
                        currentSpecifier = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::modifier, ParserState::exprstmt})){
                        currentModifier = ctx.currentToken;
                    }
                }
            };
            closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx){
                NotifyAll(ctx);
                currentLine.pop_back();
                currentLine.clear();
                exprDataSet.dataSet.clear();
                data.clear();
            };

        }
};
#endif