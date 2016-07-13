#include <srcSAXEventDispatch.hpp>
namespace srcSAXEventDispatch{
        void srcSAXEventDispatcher::AddListener(Listener* l){
            mListeners.push_back(l);
        }
        void srcSAXEventDispatcher::RemoveListener(Listener* l){
            mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), l), mListeners.end());
        }
        void srcSAXEventDispatcher::DispatchEvent(ParserState pstate, ElementState estate, std::vector<unsigned short int> tfield){
            for(std::vector<Listener*>::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener ){
                (*listener)->HandleEvent(pstate, estate, tfield);
            }
        }
}
