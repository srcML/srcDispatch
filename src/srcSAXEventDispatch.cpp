#include <srcSAXEventDispatch.hpp>
namespace srcSAXEventDispatch{
        void srcSAXEventDispatcher::addListener(Listener* l){
            mListeners.push_back(l);
        }
        void srcSAXEventDispatcher::removeListener(Listener* l){
            mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), l), mListeners.end());
        }
        void srcSAXEventDispatcher::dispatchEvent(ParserState pstate, ElementState estate){
            for(std::vector<Listener*>::iterator listener = mListeners.begin(); listener != mListeners.end(); ++listener ){
                (*listener)->handleEvent(pstate, estate);
            }
        }
}
