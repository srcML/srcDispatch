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
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            close_event_map[ParserState::modifier] = [this](const srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::parameter)){
                    if(currentModifier == "*"){
                        data.isPointer = true;
                    }
                    else if(currentModifier == "&"){
                        data.isReference = true;
                    }
                }
            };

            close_event_map[ParserState::decl] = [this](const srcSAXEventContext& ctx){
                if(ctx.And({ParserState::parameter})){
                    data.linenumber = ctx.currentLineNumber;
                    data.nameofidentifier = currentDeclName;
                }
            };

            close_event_map[ParserState::type] = [this](const srcSAXEventContext& ctx){
                if(ctx.And({ParserState::parameter})){
                    data.nameoftype = currentTypeName;
                }
            };

            close_event_map[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx){
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
            };

            close_event_map[ParserState::specifier] = [this](const srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::parameter)){
                    if(currentSpecifier == "const"){
                        data.isStatic = true;
                    }
                    else if(currentSpecifier == "static"){
                        data.isConst = true;
                    }
                }
            };

        }
};