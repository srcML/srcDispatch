#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <stack>
#include <list>
/*
 *Record current function being called
 *Record argument names and positions
 */
class CallPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    /*
    {CalledFunction1{arg1, line#}, {arg2, line#}, ..., {argn, line#},
        NestedCalledFunction1{arg1, line#},{arg2, line#}, ..., {argn, line#}
        }
    */
    public:
        struct CallData{
            void clear(){
                fnName.clear();
                callargumentlist.clear();
            }
            std::string fnName;
            std::list<std::string> callargumentlist;
        };
        ~CallPolicy(){}
        CallPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
    
            //data.name = policy->Data<NamePolicy::NameData>();
    
        }
    protected:
        void * DataInner() const override {
            return new CallData(data);
        }
    private:
        CallData data;
        std::string currentTypeName, currentCallName, currentModifier, currentSpecifier;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx){
                if(ctx.IsClosed(ParserState::call)){
                    data.callargumentlist.push_back(")");
                    NotifyAll(ctx);
                    data.callargumentlist.clear();
                    data.fnName.clear();
                }else{
                    data.callargumentlist.push_back(")");
                }
            };

            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                if(currentModifier == "*"){}
                else if(currentModifier == "&"){}
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                if(ctx.IsOpen(ParserState::name) && ctx.IsGreaterThan(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                    data.fnName = ctx.currentToken;
                    data.callargumentlist.push_back("(");
                    data.callargumentlist.push_back(ctx.currentToken);
                }
                //std::cerr<<ctx.IsOpen(ParserState::name)<<" "<<ctx.IsOpen(ParserState::argument)<<" "<<ctx.IsOpen(ParserState::argumentlist)<<" "<<ctx.IsOpen(ParserState::genericargumentlist)<<std::endl;
                if(ctx.And({ParserState::name, ParserState::argument, ParserState::argumentlist}) && ctx.IsEqualTo(ParserState::call,ParserState::argumentlist) && ctx.IsClosed(ParserState::genericargumentlist)){
                    data.callargumentlist.push_back(ctx.currentToken);
                }
            };
        }
};