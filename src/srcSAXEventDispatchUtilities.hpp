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
    class Listener{
        public:
            virtual void HandleEvent() = 0;
            virtual void HandleEvent(ParserState, ElementState, const srcSAXEventContext&) = 0;
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