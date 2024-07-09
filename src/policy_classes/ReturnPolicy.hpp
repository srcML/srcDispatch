#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <set>
#include <vector>
#include <map>
#ifndef RETURNPOLICY
#define RETURNPOLICY
class ReturnPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:

        ReturnPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        ~ReturnPolicy(){}

        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} // doesn't use other parsers
        void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} // doesn't use other parsers

        std::unordered_map<std::string, std::vector<unsigned int>>* GetReturnUses() {
            return &returnUses;
        }
    protected:
        void * DataInner() const override {}
        
    private:
        std::unordered_map<std::string, std::vector<unsigned int>> returnUses; // variablename | line number

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            
            closeEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                if(ctx.IsOpen({ParserState::returnstmt}) && ctx.IsClosed({ParserState::comment})){
                    if (returnUses[ctx.currentToken].empty() || ctx.currentLineNumber != returnUses[ctx.currentToken].back()) {
                        returnUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                    }
                }
            };

        }
};
#endif