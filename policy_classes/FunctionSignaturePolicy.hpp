#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <unordered_map>
class FunctionSignaturePolicy : public srcSAXEventDispatch::Listener{
    struct SignatureData{
        std::string nameoftype;
        std::string nameofidentifier;
        bool isConst;
        bool isReference;
        bool isPointer;
        bool isStatic;
    };
    public:
        SignatureData data;
        ~FunctionSignaturePolicy(){}
        FunctionSignaturePolicy(){InitializeEventHandlers();}
        void HandleEvent() override {}
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
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
                    if(currentModifier.first == "*"){}
                    else if(currentModifier.first == "&"){}
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
                } },
                { ParserState::specifier, [this](const srcSAXEventContext& ctx){
                } }
            };
        }
};