#include <srcDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <set>
#include <vector>
#include <map>
#ifndef RETURNPOLICY
#define RETURNPOLICY
class ReturnPolicy : public srcDispatch::EventListener, public srcDispatch::PolicyDispatcher, public srcDispatch::PolicyListener {
    public:

        ReturnPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners = {}): srcDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        ~ReturnPolicy(){}

        void Notify(const PolicyDispatcher * policy [[maybe_unused]], const srcDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} // doesn't use other parsers
        void NotifyWrite(const PolicyDispatcher * policy [[maybe_unused]], srcDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} // doesn't use other parsers

        void ClearCollection() { returnUses.clear(); }

        std::unordered_map<std::string, std::set<unsigned int>>* GetReturnUses() {
            return &returnUses;
        }
    protected:
        std::any DataInner() const override {}
        
    private:
        std::unordered_map<std::string, std::set<unsigned int>> returnUses; // variablename | line number

        void InitializeEventHandlers(){
            using namespace srcDispatch;
            
            closeEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                if(ctx.IsOpen({ParserState::returnstmt}) && ctx.IsClosed({ParserState::comment})){
                    returnUses[ctx.currentToken].insert(ctx.currentLineNumber);

                    NotifyAll(ctx);
                }
            };

        }
};
#endif