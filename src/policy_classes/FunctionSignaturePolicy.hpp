#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <ParamTypePolicy.hpp>
#include <exception>

class FunctionSignaturePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    struct SignatureData{
        std::string nameoftype;
        std::string nameofidentifier;
        bool isConst;
        bool isReference;
        bool isPointer;
        bool isStatic;
    };
    public:
        ~FunctionSignaturePolicy(){}
        FunctionSignaturePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            AddListener(&ptdata);
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
        ParamTypePolicy ptdata;
        SignatureData data;
        size_t currentArgPosition;       
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
                if(currentModifier == "*") {}
                else if(currentModifier == "&") {}
            };
            closeEventMap[ParserState::functiondecl] = [this](srcSAXEventContext& ctx){//incomplete. Blocks count too.
                NotifyAll(ctx);
            };
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //std::cerr<<"current: "<<ctx.currentToken<<std::endl;
                //std::cerr<<ctx.IsOpen(ParserState::modifier)<<" "<<ctx.IsOpen(ParserState::type)<<" "<<ctx.IsOpen(ParserState::functionblock)<<" "<<ctx.IsOpen(ParserState::genericargumentlist)<<" "<<ctx.IsOpen(ParserState::parameterlist)<<std::endl;
                if(ctx.And({ParserState::name, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Fname: "<<ctx.currentToken<<std::endl;
                }
                if(ctx.And({ParserState::name, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Typename: "<<ctx.currentToken<<std::endl;
                }
                if(ctx.And({ParserState::modifier, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Type mod: "<<ctx.currentToken<<std::endl;
                }
                if(ctx.And({ParserState::specifier, ParserState::function}) && ctx.Nor({ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                    std::cerr<<"Func mod: "<<ctx.currentToken<<std::endl;
                }
            };
        }

};