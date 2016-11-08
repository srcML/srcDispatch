#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <set>
#include <vector>
#include <cctype>
class StereotypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:
        struct StereotypeData{
            StereotypeData() {}
            void clear(){stereotypes.clear();}
            std::vector<std::string> stereotypes;
        };
        ~StereotypePolicy(){}
        StereotypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    protected:
        void * DataInner() const override {
            return new StereotypeData(data);
        }
    private:
        StereotypeData data;
        std::string currentStereotype;
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            closeEventMap[ParserState::stereotype] = [this](srcSAXEventContext& ctx){
                std::string word;
                for(char ch : currentStereotype){
                    switch(ch){
                        //it's a space, so push back stereotype and clear word
                        case ' ':{
                            data.stereotypes.push_back(word);
                            word.clear();
                            break;
                        }
                        //it's the end of the string, so push back whatever is in word
                        case 0:{
                            data.stereotypes.push_back(word);
                            break;
                        }
                        //append ch to word and keep going
                        default:{
                            if(std::isalnum(ch)){
                                word+=ch;
                            }
                            break;
                        }
                    }
                }
                NotifyAll(ctx);
                data.clear();
            };
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx){
                //TODO: possibly, this if-statement is suppressing more than just unmarked whitespace. Investigate.
                if(!(ctx.currentToken.empty() || ctx.currentToken == " ")){
                    if(ctx.IsOpen(ParserState::stereotype)){
                        currentStereotype = ctx.currentToken;
                    }
                }
            };
        }
};