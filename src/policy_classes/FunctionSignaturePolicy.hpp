#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <vector>
#include <ParamTypePolicy.hpp>
class FunctionSignaturePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    struct SignatureData{
        std::string returntype;
        std::string functionName;
        std::vector<std::pair<std::string, std::string>> parameters;
        std::string returntypeModifier;
        bool isConst;
        bool isMethod;
        bool isStatic;
    };
    public:
        ~FunctionSignaturePolicy(){}
        FunctionSignaturePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            currentArgPosition = 1;
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
    
            //data.name = policy->Data<NamePolicy::NameData>();
    
        }
    protected:
        void * DataInner() const {
            return new SignatureData(data);
        }
    private:
        ParamTypePolicy tdata;
        SignatureData data;
        size_t currentArgPosition;       
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                ctx.AddListenerNoDispatch(&tdata);
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
                if(currentModifier == "*") {}
                else if(currentModifier == "&") {}
            };
            closeEventMap[ParserState::functiondecl] = [this](srcSAXEventContext& ctx){//incomplete. Blocks count too.
                NotifyAll(ctx);
            };
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::name, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Fname: "<<ctx.currentToken<<std::endl;
                    data.functionName = ctx.currentToken;
                }
                if(ctx.And({ParserState::name, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Typename: "<<ctx.currentToken<<std::endl;
                    data.returntype = ctx.currentToken;
                }
                if(ctx.And({ParserState::modifier, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Type mod: "<<ctx.currentToken<<std::endl;
                    data.returntypeModifier = ctx.currentToken;
                }
                if(ctx.And({ParserState::specifier, ParserState::function}) && ctx.Nor({ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Func mod: "<<ctx.currentToken<<std::endl;
                    data.isConst = true;
                }
                if(ctx.And({ParserState::name, ParserState::type, ParserState::decl, ParserState::parameter}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    std::cerr<<"param type Name: "<<ctx.currentToken<<std::endl;
                }
                if(ctx.And({ParserState::name, ParserState::decl, ParserState::parameter}) && 
                    ctx.Nor({ParserState::type, ParserState::index/*skip array portion*/, ParserState::argumentlist/*skip init list portion*/, ParserState::init, ParserState::specifier, ParserState::modifier}) && 
                    !ctx.sawgeneric){
                    std::cerr<<"param decl Name: "<<ctx.currentToken<<std::endl;
                }
                if(ctx.And({ParserState::specifier, ParserState::type, ParserState::parameter})){
                    std::cerr<<"param specifier Name: "<<ctx.currentToken<<std::endl;
                }
                if(ctx.And({ParserState::modifier, ParserState::type, ParserState::parameter})){
                    std::cerr<<"param modified Name: "<<ctx.currentToken<<std::endl;
                }
            };
        }

};