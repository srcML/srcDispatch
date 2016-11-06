#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <SNLPolicy.hpp>
#include <ExprPolicy.hpp>
#include <stack>
class NLContextPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        struct NLContextData{
            NLContextData(){}
            void clear(){
                category.clear();
                identifiername.clear();
            }
            std::string category;
            std::string identifiername;
        };
        NLContextData data;
        ~NLContextPolicy(){}
        NLContextPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            sourcenlpolicy.AddListener(this);
            exprpolicy.AddListener(this);
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            using namespace srcSAXEventDispatch;
            if(ctx.IsOpen(ParserState::declstmt) && ctx.IsClosed(ParserState::exprstmt)){
                sourcenlpdata = *policy->Data<SourceNLPolicy::SourceNLData>();
                std::string top;
                if(!context.empty()){
                    top = context.top();
                }
                std::cerr<<"Def: "<<sourcenlpdata.identifiername<<std::endl;
            }else if(ctx.IsOpen(ParserState::exprstmt) && ctx.IsClosed(ParserState::declstmt)){
                exprdata = policy->Data<ExprPolicy::ExprDataSet>();
                for(auto deal : exprdata->dataset){
                    std::cerr<<"Use: "<<deal.second.nameofidentifier<<std::endl;
                }                
                /*for(auto deal : exprdata->dataset){
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
                }*/
            }
            //datatotest.push_back(SourceNLData);
        }
    protected:
        void * DataInner() const override {
            return new NLContextData(data);
        }
    private:
        SourceNLPolicy sourcenlpolicy;
        SourceNLPolicy::SourceNLData sourcenlpdata;
        ExprPolicy exprpolicy;
        ExprPolicy::ExprDataSet* exprdata;
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        std::stack<std::string> context;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->AddListener(&sourcenlpolicy);
            };
            openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->AddListener(&exprpolicy);
            };
            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                context.push("while");
            };
            openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
                context.push("for");
            };
            openEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
                context.push("if");
            };
            /*
            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                context.push("else if");
            };
            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                context.push("else");
            };*/
            closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx){
                ctx.dispatcher->RemoveListener(&sourcenlpolicy);
                data.clear();
            };
            closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx){
                ctx.dispatcher->RemoveListener(&exprpolicy);
                data.clear();
            };
            closeEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx){
                context.pop();
            };
            closeEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx){
                context.pop();
            };
            closeEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx){
                context.pop();
            };

        }
};