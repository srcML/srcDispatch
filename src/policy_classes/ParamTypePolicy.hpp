#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>

class ParamTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        struct ParamData{
            ParamData(): linenumber{0}, isConst{false}, isConstAlias{false}, isAliasToConst{false}, isReference{false}, isPointer{false}, isStatic{false} {}
            void clear(){
                nameoftype.clear();
                nameofidentifier.clear();
                namespaces.clear();
                linenumber = -1;
                isConst = false;
                isReference = false;
                isPointer = false;
                isStatic = false;
            }
            std::string nameoftype;
            std::string nameofidentifier;
            std::vector<std::string> namespaces;
            int linenumber;
            bool isConst;
            bool isConstAlias;
            bool isAliasToConst;
            bool isReference;
            bool isPointer;
            bool isStatic;
        };
        ParamTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
    
            //data.name = policy->Data<NamePolicy::NameData>();
    
        }
    protected:
        void * DataInner() const override {
            return new ParamData(data);
        }
    private:
        ParamData data;
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::type, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    data.namespaces.push_back(ctx.currentToken);
                }
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::parameter)){
                    if(currentModifier == "*"){
                        data.isPointer = true;
                    }
                    else if(currentModifier == "&"){
                        data.isReference = true;
                    }
                }
            };

            closeEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::parameter})){
                    data.linenumber = ctx.currentLineNumber;
                    data.nameofidentifier = currentDeclName;
                }
            };

            closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::parameter})){
                    data.nameoftype = currentTypeName;
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken[0] == ' ')){
                    if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                        currentTypeName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::name, ParserState::decl, ParserState::parameter}) && 
                       ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, 
                        ParserState::init, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                        currentDeclName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::specifier, ParserState::decl, ParserState::parameter})){
                        currentSpecifier = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::modifier, ParserState::type, ParserState::parameter})){
                        currentModifier = ctx.currentToken;
                    }
                }
            };
            closeEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx){
                NotifyAll(ctx);
                data.clear();
            };
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::parameter)){
                    if(currentSpecifier == "static"){
                        data.isStatic = true;
                    }
                    if(currentSpecifier == "const"){
                        data.isConst = true;
                    }
                }
                currentSpecifier.clear();
            };

        }
};