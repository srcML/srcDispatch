#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <SNLPolicy.hpp>
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
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            sourcenlpdata = policy->Data<SourceNLPolicy::SourceNLData>();
            std::string top;
            if(!context.empty()){
                top = context.top();
            }
            std::cerr<<"Output: "<<sourcenlpdata->identifiername<<" "<<sourcenlpdata->category<<" "<<top<<std::endl;
            //datatotest.push_back(SourceNLData);
        }
    protected:
        void * DataInner() const override {
            return new NLContextData(data);
        }
    private:
        SourceNLPolicy sourcenlpolicy;
        SourceNLPolicy::SourceNLData* sourcenlpdata;
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        std::stack<std::string> context;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->AddListener(&sourcenlpolicy);
            };
            openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
                std::cerr<<"seen while"<<std::endl;
                context.push("while");
            };
            openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
                std::cerr<<"seen for"<<std::endl;
                context.push("for");
            };
            openEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
                std::cerr<<"seen if"<<std::endl;
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