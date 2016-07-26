#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>

class DeclTypePolicy : public srcSAXEventDispatch::EventListener{
    struct TypeData{
        std::string nameoftype;
        std::string nameofidentifier;
        int linenumber;
        bool isConst;
        bool isReference;
        bool isPointer;
        bool isStatic;
    };
    public:
        TypeData data;
        ~DeclTypePolicy(){}
        DeclTypePolicy(){InitializeEventHandlers();}
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            closeEventMap[ParserState::modifier] = [this](const srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::declstmt)){
                    if(currentModifier == "*"){
                        data.isPointer = true;
                    }
                    else if(currentModifier == "&"){
                        data.isReference = true;
                    }
                }
            };

            closeEventMap[ParserState::decl] = [this](const srcSAXEventContext& ctx){
                if(ctx.And({ParserState::declstmt})){
                    data.linenumber = ctx.currentLineNumber;
                    data.nameofidentifier = currentDeclName;
                }
            };

            closeEventMap[ParserState::type] = [this](const srcSAXEventContext& ctx){
                if(ctx.And({ParserState::declstmt})){
                    data.nameoftype = currentTypeName;
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken[0] == ' ')){
                    if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::declstmt}) && ctx.Nor({ParserState::specifier, ParserState::modifier}) && !ctx.sawgeneric){
                        currentTypeName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::decl, ParserState::declstmt}) && 
                       ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, ParserState::init, ParserState::specifier, ParserState::modifier}) && 
                       !ctx.sawgeneric){
                        currentDeclName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::specifier, ParserState::type, ParserState::declstmt})){
                        currentSpecifier = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::modifier, ParserState::type, ParserState::declstmt})){
                        currentModifier = ctx.currentToken;
                    }
                }
            };

            closeEventMap[ParserState::specifier] = [this](const srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::declstmt)){
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