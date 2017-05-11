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
            ExprData() {}
            void clear(){
               def.clear();
               use.clear();
            }
            std::string nameofidentifier;
            std::set<unsigned int> def;
            std::set<unsigned int> use; //could be used multiple times in same expr
        };
        struct ExprDataSet{
           ExprDataSet() = default;
           ExprDataSet(std::map<std::string, ExprData> dat){
            dataset = dat;
           }
           void clear(){
            dataset.clear();
           }
           std::map<std::string, ExprData> dataset;
        };
        std::map<std::string, ExprData> dataset;
        ~ExprPolicy(){}
        ExprPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            seenAssignment = false;
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {
            return new ExprDataSet(dataset);
        }
    private:
        ExprData data;
        std::string currentTypeName, currentExprName, currentModifier, currentSpecifier;
        std::vector<unsigned int> currentLine;
        bool seenAssignment;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.currentToken == "="){
                    auto it = dataset.find(currentExprName);
                    if(it != dataset.end()){
                        it->second.use.erase(currentLine.back());
                        it->second.def.insert(currentLine.back());
                    }else{
                        std::cerr<<"No such thing as: "<<currentExprName<<std::endl;
                    }
                    seenAssignment = true;
                }
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                
            };

            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx){
                if(currentLine.empty() || currentLine.back() != ctx.currentLineNumber){
                    currentLine.push_back(ctx.currentLineNumber);
                }
                if(ctx.IsOpen({ParserState::exprstmt})){
                    auto it = dataset.find(currentExprName);
                    if(it != dataset.end()){
                        it->second.use.insert(currentLine.back()); //assume it's a use
                    }else{
                        data.nameofidentifier = currentExprName;
                        data.use.insert(currentLine.back());
                        dataset.insert(std::make_pair(currentExprName, data));
                    }
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken == " ")){
                    if(ctx.IsOpen(ParserState::exprstmt)){

                    }
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
                seenAssignment = false;
                currentLine.clear();
                dataset.clear();
                data.clear();
            };

        }
};
#endif