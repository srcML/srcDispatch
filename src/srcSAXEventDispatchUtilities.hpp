#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <initializer_list>


#ifndef INCLUDED_SRCSAX_EVENT_DISPATCH_UTILITIES_HPP
#define INCLUDED_SRCSAX_EVENT_DISPATCH_UTILITIES_HPP

namespace srcSAXEventDispatch{
    template <typename ...policies>
    class srcSAXEventDispatcher;            
    enum ElementState {open, close};
    enum ParserState {decl, expr, parameter, declstmt, exprstmt, parameterlist, 
        argumentlist, argumentlisttemplate, call, templates, ctrlflow, endflow, genericargumentlist,
        name, function, functiondecl, constructor, constructordecl, destructordecl, destructor,
        argument, index, block, type, init, op, literal, modifier, memberlist, classn, structn,
        super_list, super, preproc, whilestmt, forstmt, ifstmt, nonterminal, macro, classblock, functionblock,
        specifier, typedefexpr, empty, tokenstring, MAXENUMVALUE = empty};
    class srcSAXEventContext{
        public:
            srcSAXEventContext(){
                triggerField = std::vector<unsigned short int>(MAXENUMVALUE, 0);
            }
            std::vector<int> genericDepth;
            unsigned int currentLineNumber;
            std::vector<unsigned short int> triggerField;
            std::string currentFilePath, currentFileName, currentFileLanguage, currentsrcMLRevision, currentToken;
            bool sawgeneric;
            std::size_t depth;
            
            inline bool And(std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) continue;
                    else return false;
                }
                return true;
            }
            inline bool Nand(std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) return false;
                    else continue;
                }
                return true;
            }
            inline bool Or(std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) return true;
                    else continue;
                }
                return false;
            }
            inline bool Nor(std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) return false;
                    else continue;
                }
                return true;
            }
            inline bool IsEqualTo(ParserState lhs, ParserState rhs) const{
                return triggerField[lhs] == triggerField[rhs] ? true : false;
            }
            inline bool IsGreaterThan(ParserState lhs, ParserState rhs) const{
                return triggerField[lhs] > triggerField[rhs] ? true : false;
            }
            inline bool IsGreaterThanOrEqualTo(ParserState lhs, ParserState rhs) const{
                return triggerField[lhs] >= triggerField[rhs] ? true : false;   
            }
            inline bool IsLessThan(ParserState lhs, ParserState rhs) const{
                return triggerField[lhs] < triggerField[rhs] ? true : false;    
            }
            inline bool IsLessThanOrEqualTo(ParserState lhs, ParserState rhs) const{
                return triggerField[lhs] <= triggerField[rhs] ? true : false;   
            }
            inline bool IsOpen(ParserState field) const{
                if(triggerField[field]) return true;
                else return false;
            }
            inline bool IsClosed(ParserState field) const{
                if(triggerField[field]) return false;
                else return true;
            }
    };
    class EventListener {

        protected:
           std::unordered_map<srcSAXEventDispatch::ParserState, std::function<void(const srcSAXEventDispatch::srcSAXEventContext&)>, std::hash<int>> openEventMap, closeEventMap;


        public:

            EventListener() {
                DefaultEventHandlers();
            }

            virtual void HandleEvent() {}
            virtual void HandleEvent(srcSAXEventDispatch::ParserState pstate, srcSAXEventDispatch::ElementState estate, const srcSAXEventDispatch::srcSAXEventContext& ctx) {

                switch(estate){

                    case srcSAXEventDispatch::ElementState::open: {
                        auto event = openEventMap.find(pstate);
                        if(event != openEventMap.end()){
                            event->second(ctx);
                        }
                        break;
                    }

                    case srcSAXEventDispatch::ElementState::close: {
                        auto event = closeEventMap.find(pstate);
                        if(event != closeEventMap.end()){
                            event->second(ctx);
                        }
                        break;
                    }

                    default:
                        throw std::runtime_error("Something went terribly, terribly wrong");

                }

            }

        private:

            void DefaultEventHandlers() {

                using namespace srcSAXEventDispatch;
                openEventMap = {
                    { ParserState::declstmt, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::exprstmt, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::parameterlist, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::ifstmt, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::forstmt, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::whilestmt, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::templates, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::argumentlist, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::call, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::function, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::constructor, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::functiondecl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::destructordecl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::constructordecl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::classn, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::structn, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::destructor, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::parameter, [this](const srcSAXEventContext& ctx) {
                    } },                
                    { ParserState::memberlist, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::index, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::op, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::block, [this](const srcSAXEventContext& ctx) { 
                    } },
                    { ParserState::init, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::argument, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::literal, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::modifier, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::decl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::type, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::typedefexpr, [this](const srcSAXEventContext& ctx) {
                    } },          
                    { ParserState::expr, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::name, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::macro, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::specifier, [this](const srcSAXEventContext& ctx) {
                    } }
                };

                closeEventMap = {
                    { ParserState::declstmt, [this](const srcSAXEventContext& ctx) {
                    } },             
                    { ParserState::exprstmt, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::parameterlist, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::ifstmt, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::forstmt, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::whilestmt, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::templates, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::argumentlist, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::call, [this](const srcSAXEventContext& ctx) {
                    } },            
                    { ParserState::function, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::constructor, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::destructor, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::functiondecl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::constructordecl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::destructordecl, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::classn, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::structn, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::parameter, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::memberlist, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::index, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::op, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::block, [this](const srcSAXEventContext& ctx) { 
                    } },
                    { ParserState::init, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::argument, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::literal, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::modifier, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::decl, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::type, [this](const srcSAXEventContext& ctx) {                    
                    } },
                    { ParserState::typedefexpr, [this](const srcSAXEventContext& ctx) {
                    } },    
                    { ParserState::expr, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::name, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::macro, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::tokenstring, [this](const srcSAXEventContext& ctx) {
                    } },
                    { ParserState::specifier, [this](const srcSAXEventContext& ctx) {
                    } }
                };

        }

    };
    class PolicyDispatcher;
    class PolicyListener{

        public:

            PolicyListener() {}
            virtual void notify(const PolicyDispatcher * policy) = 0;

        };
    class EventDispatcher{
    public:
        virtual void AddListener(EventListener* l) = 0;
        virtual void RemoveListener(EventListener* l) = 0;
    protected:
        std::vector<EventListener*> elementListeners;
        virtual void DispatchEvent(ParserState, ElementState, const srcSAXEventContext&) = 0;
    };

    class PolicyDispatcher{
    public:
        PolicyDispatcher(std::initializer_list<PolicyListener *> listeners) : policyListeners(listeners){}
        virtual void AddListener(PolicyListener* listener){
            policyListeners.push_back(listener);
        }
        virtual void RemoveListener(PolicyListener* listener){
            policyListeners.erase(std::remove(policyListeners.begin(), policyListeners.end(), listener), policyListeners.end());
        }

        template<typename T>
        T * data() const {

            return static_cast<T *>(dataInner());

        }

        virtual void * dataInner() const = 0;

    protected:
        std::vector<PolicyListener*> policyListeners;
        virtual void notifyAll() {

            for( PolicyListener * const listener : policyListeners)
                listener->notify(this);

        }

    };

}

#endif
