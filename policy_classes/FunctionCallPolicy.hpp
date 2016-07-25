#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <unordered_map>
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
        void HandleEvent(){}
    private:
        std::stack<std::pair<std::string, int>> callstack;
        int currentArgPosition;
        std::string currentTypeName, currentCallName, currentModifier, currentSpecifier;
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
                { ParserState::declstmt, [this](const srcSAXEventContext& ctx){
                } },             
                { ParserState::exprstmt, [this](const srcSAXEventContext& ctx){
                } },            
                { ParserState::parameterlist, [this](const srcSAXEventContext& ctx){
                } },            
                { ParserState::ifstmt, [this](const srcSAXEventContext& ctx){
                } },            
                { ParserState::forstmt, [this](const srcSAXEventContext& ctx){
                } },            
                { ParserState::whilestmt, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::templates, [this](const srcSAXEventContext& ctx){
                } },            
                { ParserState::argumentlist, [this](const srcSAXEventContext& ctx){
                } },            
                { ParserState::call, [this](const srcSAXEventContext& ctx){
                    currentArgPosition = callstack.top().second;
                } },
                { ParserState::function, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::constructor, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::destructor, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::functiondecl, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::constructordecl, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::destructordecl, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::classn, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::structn, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::parameter, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::memberlist, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::index, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::op, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::block, [this](const srcSAXEventContext& ctx){ 
                } },
                { ParserState::init, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::argument, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::literal, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::modifier, [this](const srcSAXEventContext& ctx){
                    if(currentModifier == "*"){}
                    else if(currentModifier == "&"){}
                } },    
                { ParserState::decl, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::type, [this](const srcSAXEventContext& ctx){                    
                } },
                { ParserState::typedefexpr, [this](const srcSAXEventContext& ctx){
                } },    
                { ParserState::expr, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::name, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::macro, [this](const srcSAXEventContext& ctx){
                } },
                { ParserState::tokenstring, [this](const srcSAXEventContext& ctx){
                    if(ctx.IsOpen(ParserState::name) && ctx.IsGreaterThan(ParserState::call,ParserState::argumentlist)){
                        std::cerr<<"Call: "<<ctx.currentToken<<std::endl;
                        callstack.push(std::make_pair(ctx.currentToken, currentArgPosition));
                        currentArgPosition = 1;
                    }
                    if(ctx.And({ParserState::name, ParserState::argument, ParserState::argumentlist}) && !ctx.sawgeneric){
                        std::cerr<<"F Argument: "<<ctx.currentToken<<callstack.top().second<<std::endl;
                        ++callstack.top().second;
                    }
                } },
                { ParserState::specifier, [this](const srcSAXEventContext& ctx){
                } }
            };
        }
};