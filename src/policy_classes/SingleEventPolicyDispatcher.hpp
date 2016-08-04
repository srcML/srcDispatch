#include <srcSAXEventDispatch.hpp>

#ifndef INCLUDED_SINGLE_EVENT_POLICY_DISPATCHER_HPP
#define INCLUDED_SINGLE_EVENT_POLICY_DISPATCHER_HPP

namespace srcSAXEventDispatch {
    template <typename ...policies>
    class SingleEventPolicyDispatcher : public srcSAXEventDispatcher<policies...> {

    private:
        bool dispatched;

    public:

       SingleEventPolicyDispatcher(policies*... t2) : srcSAXEventDispatcher<policies...>{t2...}, dispatched(false) {}
        void AddListener(EventListener * listener) override {
            EventDispatcher::elementListeners.push_back(listener);
        }
        void AddListenerDispatch(EventListener * listener) override {
            EventDispatcher::elementListeners.push_back(listener);
            dispatched = false;
        }
        void AddListenerNoDispatch(EventListener * listener) override {
            AddListener(listener);
        }
        void RemoveListener(EventListener * listener) override {
            EventDispatcher::elementListeners.pop_back();
        }
        void RemoveListenerDispatch(EventListener * listener) override {
            EventDispatcher::elementListeners.pop_back();
            dispatched = false;
        }
        void RemoveListenerNoDispatch(EventListener * listener) override {
            RemoveListener(listener);
        }
    protected:
        virtual void DispatchEvent(srcSAXEventDispatch::ParserState pstate, srcSAXEventDispatch::ElementState estate) override {

            while(!dispatched) {

                dispatched = true;

                EventDispatcher::elementListeners.back()->HandleEvent(pstate, estate, EventDispatcher::ctx);

            }

            dispatched = false;

        }

    };

}

#endif
