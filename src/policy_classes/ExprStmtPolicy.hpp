#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <set>
#include <vector>
class ExprPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        struct ExprData{
            ExprData() {}
            void clear(){
               def.clear();
               use.clear();
            }
            std::string nameofidentifier;
            std::set<unsigned int> def;
            std::set<unsigned int> use; //could be used multiple times in same expr
        };
        std::map<std::string, ExprData> dataset;
        ~ExprPolicy(){}
        ExprPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            seenAssignment = false;
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {
            return new ExprData(data);
        }
    private:
        ExprData data;
        std::string currentTypeName, currentExprName, currentModifier, currentSpecifier;
        std::vector<unsigned int> currentLine;
        bool seenAssignment;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx){
                if(ctx.currentToken == "="){
                    auto it = dataset.find(currentExprName);
                    if(it != dataset.end()){
                        it->second.use.erase(currentLine.back());
                        it->second.def.insert(currentLine.back());
                        currentLine.pop_back();
                    }else{
                        std::cerr<<"No such thing as: "<<currentExprName<<std::endl;
                    }
                    seenAssignment = true;
                    std::cerr<<"Back: "<<currentLine.back()<<std::endl;
                }
            };
            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx){
                
            };

            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx){
                if(ctx.And({ParserState::exprstmt})){
                    auto it = dataset.find(currentExprName);
                    if(it != dataset.end()){
                        it->second.use.insert(currentLine.back()); //assume it's a use
                    }else{
                        data.nameofidentifier = currentExprName;
                        data.use.insert(currentLine.back());
                        dataset.insert(std::make_pair(currentExprName, data));
                    }
                }
            };

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken[0] == ' ')){
                    currentLine.push_back(ctx.currentLineNumber);
                    if(ctx.And({ParserState::name, ParserState::expr, ParserState::exprstmt}) && ctx.Nor({ParserState::specifier, ParserState::modifier, ParserState::op})){
                        currentExprName = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::specifier, ParserState::expr, ParserState::exprstmt})){
                        currentSpecifier = ctx.currentToken;
                    }
                    if(ctx.And({ParserState::modifier, ParserState::exprstmt})){
                        currentModifier = ctx.currentToken;
                    }
                }
            };
            closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx){
                NotifyAll(ctx);
                for(auto deal : dataset){
                    std::cerr<<deal.second.nameofidentifier<<std::endl;
                    std::cerr<<"def { ";
                    for(auto num : deal.second.def){
                        std::cerr<<num<<",";
                    }
                    std::cerr<<"}"<<std::endl;
                    std::cerr<<"use { ";
                    for(auto num : deal.second.use){
                        std::cerr<<num<<",";
                    }
                    std::cerr<<"}"<<std::endl;
                }
                seenAssignment = false;
                currentLine.clear();
                data.clear();
            };

        }
};