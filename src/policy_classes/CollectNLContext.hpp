/**
 * @file CollectNLContext.hpp
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
#include <srcDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <SNLPolicy.hpp>
#include <ExprPolicy.hpp>
#include <StereotypePolicy.hpp>
#include <stack>

#ifndef NLCONTEXTPOLICY
#define NLCONTEXTPOLICY
class NLContextPolicy : public srcDispatch::EventListener, public srcDispatch::PolicyDispatcher, public srcDispatch::PolicyListener {
    public:
        struct NLSet{
            NLSet(std::string idname, std::string acategory, std::string acontext, std::string astereo){
                name = idname;
                category = acategory;
                context = acontext;
                stereotype = astereo;
            }
            std::string category;
            std::string name;
            std::string context;
            std::string stereotype;
        };
        struct NLContextData{
            NLContextData(){}
            void clear(){
                category.clear();
                identifiername.clear();
            }
            std::string category;
            std::string identifiername;
            std::list<NLSet> nlsetmap;
        };
        std::map<std::string, std::string> identifierposmap;
        NLContextData data;
        ~NLContextPolicy(){}
        NLContextPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners = {}): srcDispatch::PolicyDispatcher(listeners){
            sourcenlpolicy.AddListener(this);
            exprpolicy.AddListener(this);
            stereotypepolicy.AddListener(this);
            InitializeEventHandlers();
        }
        void NotifyWrite(const PolicyDispatcher * policy [[maybe_unused]], srcDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} //doesn't use other parsers
        void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override {
            using namespace srcDispatch;
            if(ctx.IsOpen(ParserState::declstmt) && ctx.IsClosed(ParserState::exprstmt)){
                sourcenlpdata = *policy->Data<SourceNLPolicy::SourceNLData>();
                std::string top;
                if(!context.empty()){
                    top = context.top();
                }
                auto it = identifierposmap.find(sourcenlpdata.identifiername);
                if(it == identifierposmap.end()){
                    identifierposmap.insert(std::make_pair(sourcenlpdata.identifiername, sourcenlpdata.category));
                }else{
                    if(it->second != sourcenlpdata.category){   
                        it->second = "multiple";
                    }
                }
                //std::cerr<<"Def: "<<sourcenlpdata.identifiername<<std::endl;
            }else if(ctx.IsOpen(ParserState::exprstmt) && ctx.IsClosed(ParserState::declstmt)){
                exprdata = *policy->Data<ExprPolicy::ExprDataSet>();
                std::string top;
                if(!context.empty()){
                    top = context.top();
                }else{
                    top = "none";
                }
                std::string stereo;
                if(!stereotype.stereotypes.empty()){
                    stereo = stereotype.stereotypes.front();
                }else{
                    stereo = "none";
                }
                for(auto deal : exprdata.dataset){
                    auto it = identifierposmap.find(deal.second.nameofidentifier);
                    if(it != identifierposmap.end()){
                        std::string categorystr;
                        if(it->second.empty()){
                            categorystr = "none";
                        }else{
                            categorystr = it->second;
                        }
                        NLSet nlset = NLSet(deal.second.nameofidentifier,categorystr,top,stereo);
                        data.nlsetmap.push_back(nlset);
                    }
                }
            }
            if(ctx.IsOpen(ParserState::stereotype)){
                stereotype = *policy->Data<StereotypePolicy::StereotypeData>();
            }
            //datatotest.push_back(SourceNLData);
        }
    protected:
        void * DataInner() const override {
            return new NLContextData(data);
        }
    private:
        SourceNLPolicy sourcenlpolicy;
        SourceNLPolicy::SourceNLData sourcenlpdata;
        ExprPolicy exprpolicy;
        ExprPolicy::ExprDataSet exprdata;
        StereotypePolicy stereotypepolicy;
        StereotypePolicy::StereotypeData stereotype;
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        std::stack<std::string> context;
        void InitializeEventHandlers(){
            using namespace srcDispatch;
            openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->AddListenerDispatch(&sourcenlpolicy);
            };
            closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                ctx.dispatcher->RemoveListenerDispatch(&sourcenlpolicy);
                ///data.clear();
            };

            openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->AddListenerDispatch(&exprpolicy);
            };
            closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx){
                ctx.dispatcher->RemoveListenerDispatch(&exprpolicy);
                //data.clear();
            };

            openEventMap[ParserState::stereotype] = [this](srcSAXEventContext& ctx){
                ctx.dispatcher->AddListenerDispatch(&stereotypepolicy);
            };
            closeEventMap[ParserState::stereotype] = [this](srcSAXEventContext& ctx){
                ctx.dispatcher->RemoveListenerDispatch(&stereotypepolicy);
            };

            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                context.push("while");
            };
            closeEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx){
                context.pop();
            };
            
            openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
                context.push("for");
            };
            closeEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx){
                context.pop();
            };

            openEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
                context.push("if");
            };
            closeEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx){
                context.pop();
            };
            
            /*
            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                context.push("else if");
            };
            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                context.push("else");
            };*/

            closeEventMap[ParserState::archive] = [this](srcSAXEventContext& ctx){
                NotifyAll(ctx);
            };

        }
};
#endif