#include <memory>
namespace srcSAXEventDispatch{
	#include <vector>
	template <typename ...policies>
    class srcSAXEventDispatcher;	    	
    enum ElementState {open, close};
    enum ParserState {decl, expr, parameter, declstmt, exprstmt, parameterlist, 
        argumentlist, argumentlisttemplate, call, templates, ctrlflow, endflow, 
        name, function, functiondecl, constructor, constructordecl, destructordecl, destructor,
        argument, index, block, type, init, op, literal, modifier, memberlist, classn, structn,
        preproc, whilestmt, forstmt, ifstmt, nonterminal, macro, classblock, functionblock,
        specifier, typedefexpr, empty, tokenstring, MAXENUMVALUE = empty};
	class srcSAXEventContext{
    	public:
    		srcSAXEventContext(){
    			triggerField = std::vector<unsigned short int>(MAXENUMVALUE, 0);
    		}
	
        	unsigned int currentLineNumber;
        	std::vector<unsigned short int> triggerField;
        	std::string currentFilePath, currentFileName, currentFileLanguage, currentsrcMLRevision, currentToken;
			bool sawgeneric;
			
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
    class Listener {

        private:
            std::unordered_map< std::string, std::function<void()>> open_event_map, close_event_map;

        public:
            virtual void HandleEvent() = 0;
            virtual void HandleEvent(ParserState, ElementState, const srcSAXEventContext&) = 0;

            void InitializeEventHandlers() {

                using namespace srcSAXEventDispatch;
                open_event_map = {
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

                close_event_map = {
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
    class EventDispatcher{
    public:
        virtual void AddListener(Listener* l) = 0;
        virtual void RemoveListener(Listener* l) = 0;
    protected:
        std::vector<Listener*> elementListeners;
        virtual void DispatchEvent(ParserState, ElementState, const srcSAXEventContext&) = 0;
    };
}