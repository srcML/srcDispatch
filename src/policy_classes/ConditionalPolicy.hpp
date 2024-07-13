#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <set>
#include <vector>
#include <map>
#ifndef CONDITIONALPOLICY
#define CONDITIONALPOLICY
class ConditionalPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:

        ConditionalPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        ~ConditionalPolicy(){}

        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
        void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

        std::unordered_map<std::string, std::vector<unsigned int>>* GetConditionalUses() {
            return &conditionalUses;
        }
        std::unordered_map<std::string, std::vector<unsigned int>>* GetConditionalDefs() {
            return &conditionalDefs;
        }

        std::unordered_map<std::string, std::vector<unsigned int>>* GetSwitchUses() {
            return &switchUses;
        }
        std::unordered_map<std::string, std::vector<unsigned int>>* GetSwitchDefs() {
            return &switchDefs;
        }

        std::string GetVarName() {
            return currentExprName;
        }

        void EditDepth(int d) {

            if (switchControlVars.size() > 0 && d < 0) {
                switchControlVars.erase(switchDepth);
            }
            switchDepth += d;
            if (switchDepth < 0) switchDepth = 0;
        }

    protected:
        void * DataInner() const override {}
        
    private:
        std::unordered_map<std::string, std::vector<unsigned int>> conditionalUses, conditionalDefs;
        std::unordered_map<std::string, std::vector<unsigned int>> switchUses, switchDefs;
        std::unordered_map< unsigned int, std::set<std::string> > switchControlVars;
        unsigned int switchDepth = 0;
        std::string currentExprName, currentExprOp;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            
            closeEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                currentExprName = ctx.currentToken;

                if ( ctx.IsClosed({ParserState::comment}) ) {
                    // The following is for detecting possible uses within various Conditionals
                    if( (ctx.IsOpen({ParserState::ifstmt}) || ctx.IsOpen({ParserState::elseif})) && ctx.IsOpen({ParserState::condition}) ){
                        if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                            conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                        }
                    }

                    // The following is for detecting possible uses within various Conditionals
                    if( ctx.IsOpen({ParserState::whilestmt}) && ctx.IsOpen({ParserState::condition}) ){
                        if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                            conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                        }
                    }
                    
                    // The following is for detecting possible uses within various Conditionals
                    if( ctx.IsOpen({ParserState::forstmt}) ) {
                        if ( ctx.IsOpen({ParserState::condition}) ){
                            if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                                conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        }

                        if ( ( ctx.IsOpen({ParserState::init}) || ctx.IsOpen({ParserState::incr}) || ctx.IsOpen({ParserState::decr}) ) && ctx.IsOpen({ParserState::expr}) ){
                            if (conditionalDefs[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalDefs[ctx.currentToken].back()) {
                                conditionalDefs[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }

                            if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                                conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        }
                    }
                    
                    // The following is for detecting possible uses within various Conditionals
                    if( ctx.IsOpen({ParserState::dostmt}) && ctx.IsOpen({ParserState::condition}) ){
                        if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                            conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                        }
                    }

                    // Get uses or use/defs within switch conditions
                    if( ctx.IsOpen({ParserState::switchstmt}) && ctx.IsOpen({ParserState::condition}) ){

                        switchControlVars[switchDepth].insert(ctx.currentToken);

                        if (switchUses[ctx.currentToken].empty() || ctx.currentLineNumber != switchUses[ctx.currentToken].back()) {
                            switchUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                        }

                        if (!currentExprOp.empty()) {
                            // prefix : <operator>++</operator><name>c</name>
                            if (switchDefs[ctx.currentToken].empty() || ctx.currentLineNumber != switchDefs[ctx.currentToken].back()) {
                                switchDefs[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                            
                            currentExprName = "";
                            currentExprOp = "";
                        }
                    }
                }
            };

            // Assume switch cases are a use of the condition value
            closeEventMap[ParserState::switchcase] = [this](srcSAXEventContext &ctx) {
                if( ctx.IsOpen({ParserState::switchstmt}) ){
                    for (auto varName : switchControlVars[switchDepth]) {
                        if (switchUses[varName].empty() || ctx.currentLineNumber != switchUses[varName].back()) {
                            switchUses[varName].push_back(ctx.currentLineNumber);
                        }
                    }
                }
            };

            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                // Long or-statement allows various declaration operators to get planted into
                // a slices def data output
                // if (ctx.currentToken == "=" || ctx.currentToken == "++" || ctx.currentToken == "+=" ||
                //     ctx.currentToken == "--" || ctx.currentToken == "-=" || ctx.currentToken == "*=" ||
                //     ctx.currentToken == "/=" || ctx.currentToken == "%=" || ctx.currentToken == "&=" ||
                //     ctx.currentToken == "|=" || ctx.currentToken == "^=" || ctx.currentToken == "<<=" ||
                //     ctx.currentToken == ">>=")
                bool isMutatorOp = (ctx.currentToken == "+=" || ctx.currentToken == "-=" ||
                                    ctx.currentToken == "*=" || ctx.currentToken == "/=" ||
                                    ctx.currentToken == "%=" || ctx.currentToken == "++" ||
                                    ctx.currentToken == "--");

                // Within switch blocks find use/defs
                if ( isMutatorOp && ctx.IsOpen({ParserState::switchstmt}) ) {
                    currentExprOp = ctx.currentToken;
                    if (!currentExprName.empty()) {
                        // postfix : <name>c</name><operator>++</operator>
                        if (switchDefs[currentExprName].empty() || ctx.currentLineNumber != switchDefs[currentExprName].back()) {
                            switchDefs[currentExprName].push_back(ctx.currentLineNumber);
                        }
                        
                        if (switchUses[currentExprName].empty() || ctx.currentLineNumber != switchUses[currentExprName].back()) {
                            switchUses[currentExprName].push_back(ctx.currentLineNumber);
                        }

                        currentExprName = "";
                        currentExprOp = "";
                    }
                }
            };
        }
};
#endif