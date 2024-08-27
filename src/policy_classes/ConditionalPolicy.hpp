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
        struct DvarData{
            DvarData(std::string func, std::string name, unsigned int line) {
                function = func;
                lhsName = name;
                lhsDefLine = line;
                dvars.clear();
            }
            std::string function, lhsName;
            unsigned int lhsDefLine;
            std::set<std::pair<std::string, unsigned int>> dvars;
        };

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

        void DeleteUsesCollection(std::string name) { conditionalUses.erase(name); }
        void DeleteDefsCollection(std::string name) { conditionalDefs.erase(name); }

        std::vector<DvarData>* GetPossibleDvars() { return &dvarSet; }

        const std::string& GetLastFunction() { return recentFunction; }

    protected:
        void * DataInner() const override {}
        
    private:
        std::unordered_map<std::string, std::vector<unsigned int>> conditionalUses, conditionalDefs;
        std::unordered_map<std::string, std::vector<unsigned int>> switchUses, switchDefs;
        std::unordered_map< unsigned int, std::set<std::string> > switchControlVars;
        unsigned int switchDepth = 0;
        std::string currentExprName = "", currentExprOp = "", currentType = "";
        std::vector<DvarData> dvarSet;
        std::string recentFunction;
        bool insertDvar = false;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            closeEventMap[ParserState::function] = [this](srcSAXEventContext &ctx) {
                NotifyAll(ctx);
            };
            
            closeEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                recentFunction = ctx.currentFunctionName;
                currentExprName = ctx.currentToken;

                if (ctx.IsOpen({ParserState::decl}) && ctx.IsClosed({ParserState::type}) && dvarSet.size() > 0 && insertDvar) {
                    dvarSet.back().dvars.insert( std::make_pair(ctx.currentToken, ctx.currentLineNumber) );
                }

                if ( ctx.IsClosed({ParserState::comment}) ) {
                    // The following is for detecting possible uses within various Conditionals
                    if ( (ctx.IsOpen({ParserState::ifstmt}) || ctx.IsOpen({ParserState::elseif})) && ctx.IsOpen({ParserState::condition}) ) {
                        // switch using data-type capturing:
                        // xml will appear as `<type>int</type><name>i</name>`
                        // using this switch when I hit the name when I've detected
                        // a type within an if-stmt I wont add the line as a use of
                        // a line where it is being declared:
                        // `4 | if (int a = i; i < 10) {...}` I want to mark:
                        // "a" with only def on line 4, and "i" use on 4

                        if (currentType.empty()) {    
                            if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                                conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        } else {
                            currentType.clear();
                        }
                    }

                    // The following is for detecting possible uses within various Conditionals
                    if ( ctx.IsOpen({ParserState::whilestmt}) && ctx.IsOpen({ParserState::condition}) ){
                        if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                            conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                        }
                    }
                    
                    // The following is for detecting possible uses within various Conditionals
                    if ( ctx.IsOpen({ParserState::forstmt}) ) {
                        if ( ctx.And({ParserState::decl, ParserState::init, ParserState::expr}) ) {
                            if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                                conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        }

                        if ( ctx.IsOpen({ParserState::condition}) ){
                            if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                                conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        }

                        if ( ctx.IsOpen({ParserState::incr}) || ctx.IsOpen({ParserState::decr}) ){
                            if (conditionalDefs[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalDefs[ctx.currentToken].back()) {
                                conditionalDefs[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }

                            if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                                conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        }
                    }
                    
                    // The following is for detecting possible uses within various Conditionals
                    if ( ctx.IsOpen({ParserState::dostmt}) && ctx.IsOpen({ParserState::condition}) ){
                        if (conditionalUses[ctx.currentToken].empty() || ctx.currentLineNumber != conditionalUses[ctx.currentToken].back()) {
                            conditionalUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                        }
                    }

                    // Get uses or use/defs within switch conditions
                    if ( ctx.IsOpen({ParserState::switchstmt}) && ctx.IsOpen({ParserState::condition}) ){
                        if ( ctx.IsOpen({ParserState::init}) ) {
                            if (switchUses[ctx.currentToken].empty() || ctx.currentLineNumber != switchUses[ctx.currentToken].back()) {
                                switchUses[ctx.currentToken].push_back(ctx.currentLineNumber);
                            }
                        } else {
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
                }
            };

            // Assume switch cases are a use of the condition value
            closeEventMap[ParserState::switchcase] = [this](srcSAXEventContext &ctx) {
                if ( ctx.IsOpen({ParserState::switchstmt}) ){
                    for (auto varName : switchControlVars[switchDepth]) {
                        if (switchUses[varName].empty() || ctx.currentLineNumber != switchUses[varName].back()) {
                            switchUses[varName].push_back(ctx.currentLineNumber);
                        }
                    }
                }
            };
            
            closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx){
                if (ctx.And({ParserState::ifstmt, ParserState::condition})) {
                    currentType = ctx.currentToken;
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                std::string token = ctx.currentToken;
                std::string extractedToken = "";

                // Extract white-spaces from token
                for (int i = 0; i < token.length(); ++i) {
                    if (std::isalnum(token[i]) || token[i] == '=' || token[i] == '-' ||
                        token[i] == '+' || token[i] == '*' || token[i] == '/' ||
                        token[i] == '%' || token[i] == '&') {
                        extractedToken += token[i];
                    }
                }
                
                if (extractedToken == "+=" || extractedToken == "-=" || extractedToken == "*=" ||
                    extractedToken == "/=" || extractedToken == "%=" || extractedToken == "=") {

                    if ( (ctx.IsOpen({ParserState::decl}) && ctx.IsClosed({ParserState::name}) && ctx.IsClosed({ParserState::type})) && !currentExprName.empty()) {
                        dvarSet.push_back(DvarData(ctx.currentFunctionName, currentExprName, ctx.currentLineNumber));
                        insertDvar = true;
                    }
                }
            };

            closeEventMap[ParserState::init] = [this](srcSAXEventContext& ctx){
                insertDvar = false;
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

                // Within conditional blocks find use/defs
                if ( isMutatorOp ) {
                    currentExprOp = ctx.currentToken;
                    if (!currentExprName.empty()) {
                        // postfix : <name>c</name><operator>++</operator>
                        if (conditionalDefs[currentExprName].empty() || ctx.currentLineNumber != conditionalDefs[currentExprName].back()) {
                            conditionalDefs[currentExprName].push_back(ctx.currentLineNumber);
                        }
                        
                        if (conditionalUses[currentExprName].empty() || ctx.currentLineNumber != conditionalUses[currentExprName].back()) {
                            conditionalUses[currentExprName].push_back(ctx.currentLineNumber);
                        }

                        currentExprName = "";
                        currentExprOp = "";
                    }
                }

                if (ctx.currentToken == "+=" || ctx.currentToken == "-=" ||
                    ctx.currentToken == "*=" || ctx.currentToken == "/=" ||
                    ctx.currentToken == "%=" || ctx.currentToken == "=") {
                    if ( ctx.IsOpen({ParserState::decl}) && !currentExprName.empty() && !currentType.empty() ) {
                        dvarSet.push_back(DvarData(ctx.currentFunctionName, currentExprName, ctx.currentLineNumber));
                    }
                }
            };
        }
};
#endif