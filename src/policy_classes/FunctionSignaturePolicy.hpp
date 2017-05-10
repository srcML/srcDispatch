#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <vector>
#include <ParamTypePolicy.hpp>

#ifndef FUNCTIONSIGNATUREPOLICY
#define FUNCTIONSIGNATUREPOLICY
class FunctionSignaturePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        struct SignatureData{
            SignatureData():isConst{false}, constPointerReturn{false}, isMethod{false}, isStatic{false}, pointerToConstReturn{false}, hasAliasedReturn{false} {}
            int linenumber;
            std::string returnType;
            std::string name;
            std::string returnTypeModifier;
            std::vector<std::string> functionNamespaces;
            std::vector<std::string> returnTypeNamespaces;
            std::vector<ParamTypePolicy::ParamData> parameters;
            bool isConst;
            bool isMethod;
            bool isStatic;
            bool pointerToConstReturn;
            bool constPointerReturn;
            bool hasAliasedReturn;
            void clear(){
                returnType.clear();
                name.clear();
                parameters.clear();
                returnTypeNamespaces.clear();
                functionNamespaces.clear();
                returnTypeModifier.clear();
                isConst = false;
                isMethod = false;
                isStatic = false;
                pointerToConstReturn = false;
                constPointerReturn = false;
                hasAliasedReturn = false;
            }
        };
        ~FunctionSignaturePolicy(){}
        FunctionSignaturePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            currentArgPosition = 1;
            parampolicy.AddListener(this);
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            paramdata = policy->Data<ParamTypePolicy::ParamData>();
            data.parameters.push_back(*paramdata);
        }
    protected:
        void * DataInner() const override {
            return new SignatureData(data);
        }
    private:
        bool seenModifier;
        ParamTypePolicy parampolicy;
        ParamTypePolicy::ParamData* paramdata;
        SignatureData data;
        size_t currentArgPosition;       
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                data.linenumber = ctx.currentLineNumber;
                ctx.dispatcher->AddListener(&parampolicy);
            };
            openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::type, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::functionblock, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    data.returnTypeNamespaces.push_back(ctx.currentToken);
                }
                if(ctx.IsOpen(ParserState::function) && ctx.Nor({ParserState::type, ParserState::parameterlist, ParserState::functionblock, ParserState::specifier, ParserState::modifier, ParserState::genericargumentlist})){
                    data.functionNamespaces.push_back(ctx.currentToken);
                }
            };
            openEventMap[ParserState::functionblock] = [this](srcSAXEventContext& ctx){//incomplete. Blocks count too.
                if(ctx.IsOpen(ParserState::classn)){
                    data.isMethod = true;
                }
                NotifyAll(ctx);
                seenModifier = false;
                data.clear();
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
                if(currentModifier == "*") {
                    if(ctx.And({ParserState::type, ParserState::function}) && ctx.IsClosed(ParserState::parameterlist)){
                        seenModifier = true;
                        data.hasAliasedReturn = true;
                    }
                }
                else if(currentModifier == "&") {}
            };
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::name, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::type, ParserState::parameterlist, ParserState::genericargumentlist})){
                    data.name = ctx.currentToken;
                }
                if(ctx.And({ParserState::name, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::functionblock, ParserState::parameterlist, ParserState::genericargumentlist})){
                    data.returnType = ctx.currentToken;
                }
                if(ctx.And({ParserState::modifier, ParserState::type, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::genericargumentlist})){
                    data.returnTypeModifier = ctx.currentToken;
                }
                if(ctx.And({ParserState::specifier, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::functionblock, ParserState::genericargumentlist})){
                    currentSpecifier = ctx.currentToken;
                }
                if(ctx.And({ParserState::modifier, ParserState::function}) && ctx.Nor({ParserState::parameterlist, ParserState::functionblock, ParserState::genericargumentlist})){
                    currentModifier = ctx.currentToken;
                }
            };
            closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
                if(currentSpecifier == "const" && ctx.Nor({ParserState::parameterlist, ParserState::type})){
                    data.isConst = true;
                }
                if(currentSpecifier == "const" && ctx.IsOpen(ParserState::function) && ctx.IsOpen(ParserState::type)){
                    if(!seenModifier){
                        data.pointerToConstReturn = true;
                    }else{
                        data.constPointerReturn = true;
                    }
                }
                if(currentSpecifier == "static" && ctx.Nor({ParserState::parameterlist, ParserState::type})){
                    data.isStatic = true;
                }
                currentSpecifier.clear();
            };
            closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->RemoveListener(&parampolicy);
            };
        }

};
#endif