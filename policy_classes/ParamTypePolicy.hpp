#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <unordered_map>
class ParamTypePolicy : public srcSAXEventDispatch::Listener{
    struct ParamData{
        std::string nameoftype;
        std::string nameofidentifier;
        int linenumber;
        bool isConst;
        bool isReference;
        bool isPointer;
        bool isStatic;
    };
    public:
        ParamData data;
        ~ParamTypePolicy(){}
        ParamTypePolicy(){InitializeEventHandlers();}
        void HandleEvent() override {}
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        std::unordered_map<srcSAXEventDispatch::ParserState, std::function<void(const srcSAXEventDispatch::srcSAXEventContext&)>, std::hash<int>> EventToHandlerMap;
        void HandleEvent(srcSAXEventDispatch::ParserState pstate, srcSAXEventDispatch::ElementState estate, const srcSAXEventDispatch::srcSAXEventContext& ctx) override{
            switch(estate){
                case srcSAXEventDispatch::ElementState::open:{
                    return; //do nothing
                }
                case srcSAXEventDispatch::ElementState::close:{
                    auto event = EventToHandlerMap.find(pstate);
                    if(event != EventToHandlerMap.end()){
                        event->second(ctx);
                    }
                    break;
                }
                default:
                    throw std::runtime_error("Something went terribly, terribly wrong"); 
            }
        }
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            EventToHandlerMap = {  
                { ParserState::modifier, [this](const srcSAXEventContext& ctx){
                    if(ctx.IsOpen(ParserState::parameter)){
                        if(currentModifier == "*"){
                            data.isPointer = true;
                        }
                        else if(currentModifier == "&"){
                            data.isReference = true;
                        }
                    }
                } },    
                { ParserState::decl, [this](const srcSAXEventContext& ctx){
                    if(ctx.And({ParserState::parameter})){
                        data.linenumber = ctx.currentLineNumber;
                        data.nameofidentifier = currentDeclName;
                    }
                } },    
                { ParserState::type, [this](const srcSAXEventContext& ctx){
                    if(ctx.And({ParserState::parameter})){
                        data.nameoftype = currentTypeName;
                    }
                } },
                { ParserState::tokenstring, [this](const srcSAXEventContext& ctx){
                    //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                    if(!(ctx.currentToken.empty() || ctx.currentToken[0] == ' ')){
                        if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier}) && !ctx.sawgeneric){
                            currentTypeName = ctx.currentToken;
                        }
                        if(ctx.And({ParserState::name, ParserState::decl, ParserState::parameter}) && 
                           ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, ParserState::init, ParserState::specifier, ParserState::modifier}) && 
                           !ctx.sawgeneric){
                            currentDeclName = ctx.currentToken;
                        }
                        if(ctx.And({ParserState::specifier, ParserState::type, ParserState::parameter})){
                            currentSpecifier = ctx.currentToken;
                        }
                        if(ctx.And({ParserState::modifier, ParserState::type, ParserState::parameter})){
                            currentModifier = ctx.currentToken;
                        }
                    }
                } },
                { ParserState::specifier, [this](const srcSAXEventContext& ctx){
                    if(ctx.IsOpen(ParserState::parameter)){
                        if(currentSpecifier == "const"){
                            data.isStatic = true;
                        }
                        else if(currentSpecifier == "static"){
                            data.isConst = true;
                        }
                    }
                } }
            };
        }
};