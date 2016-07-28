#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <stack>
#include <list>
/*
 *Record current function being called
 *Record argument names and positions
 */
class CallPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher {
    /*
    {CalledFunction1{arg1, line#}, {arg2, line#}, ..., {argn, line#},
        NestedCalledFunction1{arg1, line#},{arg2, line#}, ..., {argn, line#}
        }
    */
    struct CallData{
        std::string fnName;
        std::list<std::pair<std::string, unsigned int>> CallArgumentLineNumberMap;
    };
    public:
        ~CallPolicy(){}
        CallPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            currentArgPosition = 1;
            InitializeEventHandlers();
        }
    protected:
        void * DataInner() const override {
            return new CallData(data);
        }
    private:
        CallData data;
        int currentArgPosition;
        std::string currentTypeName, currentCallName, currentModifier, currentSpecifier;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                    currentArgPosition = --(data.CallArgumentLineNumberMap.back()).second;
            };

            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                    if(currentModifier == "*"){}
                    else if(currentModifier == "&"){}
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                    if(ctx.IsOpen(ParserState::name) && ctx.IsGreaterThan(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                        std::cerr<<"Call: "<<ctx.currentToken<<std::endl;
                        data.fnName = ctx.currentToken;
                        currentArgPosition = 1;
                    }
                    //std::cerr<<ctx.IsOpen(ParserState::name)<<" "<<ctx.IsOpen(ParserState::argument)<<" "<<ctx.IsOpen(ParserState::argumentlist)<<" "<<ctx.IsOpen(ParserState::genericargumentlist)<<std::endl;
                    if(ctx.And({ParserState::name, ParserState::argument, ParserState::argumentlist}) && ctx.IsEqualTo(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                        data.CallArgumentLineNumberMap.push_back(std::make_pair(ctx.currentToken, currentArgPosition));
                        std::cerr<<"F Argument: "<<ctx.currentToken<<data.CallArgumentLineNumberMap.back().second<<std::endl;
                        ++currentArgPosition;
                    }
            };
            closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                if(ctx.IsClosed(ParserState::call)){
                    NotifyAll(ctx);
                }
            };
        }
};