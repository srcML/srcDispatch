#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <stack>
/*
 *Record current function being called
 *Record argument names and positions
 */
class CallPolicy : public srcSAXEventDispatch::Listener{
    struct CallData{
        /*
        {CalledFunction1{arg1, line#}, {arg2, line#}, ..., {argn, line#},
         NestedCalledFunction1{arg1, line#},{arg2, line#}, ..., {argn, line#}
         }
        */
        std::map<std::string, std::vector<std::pair<std::string, unsigned int>>> CallArgumentLineNumberMap;
    };
    public:
        CallData data;
        ~CallPolicy(){}
        CallPolicy(){
            currentArgPosition = 1;
            InitializeEventHandlers();
        }
    private:
        std::stack<std::pair<std::string, int>> callstack;
        int currentArgPosition;
        std::string currentTypeName, currentCallName, currentModifier, currentSpecifier;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            close_event_map[ParserState::call] = [this](const srcSAXEventContext& ctx){
                    currentArgPosition = callstack.top().second;
                };

            close_event_map[ParserState::modifier] = [this](const srcSAXEventContext& ctx){
                    if(currentModifier == "*"){}
                    else if(currentModifier == "&"){}
                };

            close_event_map[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx){
                    if(ctx.IsOpen(ParserState::name) && ctx.IsGreaterThan(ParserState::call,ParserState::argumentlist)){
                        std::cerr<<"Call: "<<ctx.currentToken<<std::endl;
                        callstack.push(std::make_pair(ctx.currentToken, currentArgPosition));
                        currentArgPosition = 1;
                    }
                    if(ctx.And({ParserState::name, ParserState::argument, ParserState::argumentlist}) && !ctx.sawgeneric){
                        std::cerr<<"F Argument: "<<ctx.currentToken<<callstack.top().second<<std::endl;
                        ++callstack.top().second;
                    }
                };
        }
};