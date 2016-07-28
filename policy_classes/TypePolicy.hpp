#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>

class DeclTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher {
    struct DeclTypeData{
        std::string nameoftype;
        std::string nameofidentifier;
        int linenumber;
        bool isConst;
        bool isReference;
        bool isPointer;
        bool isStatic;
    };
    public:
        DeclTypeData data;
        ~DeclTypePolicy(){}
        DeclTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
    protected:
        void * DataInner() const override {
            return new DeclTypeData(data);
        }
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::declstmt)){
                    if(currentModifier == "*"){
                        data.isPointer = true;
                    }
                    else if(currentModifier == "&"){
                        data.isReference = true;
                    }
                }
            };

            closeEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::declstmt})){
                    data.linenumber = ctx.currentLineNumber;
                    data.nameofidentifier = currentDeclName;
                }
            };

            closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::declstmt})){
                    data.nameoftype = currentTypeName;
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
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
            closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                    NotifyAll(ctx);
            };
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx){
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