#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>

#ifndef SOURCENLPOLICY
#define SOURCENLPOLICY
class SourceNLPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        struct SourceNLData{
            SourceNLData(){}
            void clear(){
                category.clear();
                identifiername.clear();
            }
            std::string category;
            std::string identifiername;
        };
        SourceNLData data;
        ~SourceNLPolicy(){}
        SourceNLPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {
            return new SourceNLData(data);
        }
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::snoun] = [this](srcSAXEventContext& ctx){
                //std::cerr<<ctx.currentTag<<" "<<data.identifiername<<std::endl;
                data.category = "snoun";
                NotifyAll(ctx);
                data.identifiername.clear();
            };

            closeEventMap[ParserState::propersnoun] = [this](srcSAXEventContext& ctx){
                //std::cerr<<ctx.currentTag<<" "<<data.identifiername<<std::endl;
                data.category = "propersnoun";
                NotifyAll(ctx);
                data.identifiername.clear();
            };

            closeEventMap[ParserState::spronoun] = [this](srcSAXEventContext& ctx){
                //std::cerr<<ctx.currentTag<<" "<<data.identifiername<<std::endl;
                data.category = "spronoun";
                NotifyAll(ctx);
                data.identifiername.clear();
            };

            closeEventMap[ParserState::sadjective] = [this](srcSAXEventContext& ctx){
                //std::cerr<<ctx.currentTag<<" "<<data.identifiername<<std::endl;
                data.category = "sadjective";
                NotifyAll(ctx);
                data.identifiername.clear();
            };

            closeEventMap[ParserState::sverb] = [this](srcSAXEventContext& ctx){
                //std::cerr<<ctx.currentTag<<" "<<data.identifiername<<std::endl;
                data.category = "sverb";
                NotifyAll(ctx);
                data.identifiername.clear();
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(ctx.Or({ParserState::snoun, ParserState::spronoun, ParserState::propersnoun, ParserState::sverb, ParserState::sadjective})){
                    data.identifiername.append(ctx.currentToken);
                }
            };
            closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                //NotifyAll(ctx);
                data.clear();
            };

        }
};
#endif