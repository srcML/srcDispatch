#ifndef INCLUDED_SRCSAX_SINGLE_EVENT_DISPATCHER_HPP
#define INCLUDED_SRCSAX_SINGLE_EVENT_DISPATCHER_HPP

#include <srcSAXEventDispatcher.hpp>

namespace srcSAXEventDispatch {
    template <typename ...policies>
    class srcSAXSingleEventDispatcher : public srcSAXEventDispatcher<policies...> {

    private:
        bool dispatched;

    public:

       srcSAXSingleEventDispatcher(PolicyListener * listener) : srcSAXEventDispatcher<policies...>(listener), dispatched(false) {}
        virtual void AddListener(EventListener * listener) override {
            EventDispatcher::elementListeners.back()->SetDispatched(false);
            EventDispatcher::elementListeners.push_back(listener);
        }
        virtual void AddListenerDispatch(EventListener * listener) override {
            AddListener(listener);
            dispatched = false;
        }
        virtual void AddListenerNoDispatch(EventListener * listener) override {
            AddListener(listener);
        }
        virtual void RemoveListener(EventListener * listener) override {
            EventDispatcher::elementListeners.back()->SetDispatched(false);
            EventDispatcher::elementListeners.pop_back();
        }
        virtual void RemoveListenerDispatch(EventListener * listener) override {
            RemoveListener(listener);
            dispatched = false;
        }
        virtual void RemoveListenerNoDispatch(EventListener * listener) override {
            RemoveListener(listener);
        }
    protected:
        virtual void DispatchEvent(srcSAXEventDispatch::ParserState pstate, srcSAXEventDispatch::ElementState estate) override {

            while(!dispatched) {

                dispatched = true;

                EventDispatcher::elementListeners.back()->HandleEvent(pstate, estate, EventDispatcher::ctx);
                EventDispatcher::elementListeners.back()->SetDispatched(false);

            }

            dispatched = false;

        }

    };

}

#endif
