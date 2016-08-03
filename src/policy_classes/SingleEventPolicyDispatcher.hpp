#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>

#include <stack>

#ifndef INCLUDED_SINGLE_EVENT_POLICY_DISPATCHER_HPP
#define INCLUDED_SINGLE_EVENT_POLICY_DISPATCHER_HPP

class SingleEventPolicyDispatcher : public srcSAXEventDispatch::EventListener {

private:

   std::stack<srcSAXEventDispatch::EventListener *> listenerStack;



public:

    SingleEventPolicyDispatcher() : srcSAXEventDispatch::EventListener() {}

    void PushListener(srcSAXEventDispatch::EventListener * listener) {

        listenerStack.push(listener);

    }

    void PopListener(srcSAXEventDispatch::EventListener * listener) {

        listenerStack.pop();

    }

    void PushListenerDispatch(srcSAXEventDispatch::EventListener * listener) {

        listenerStack.push(listener);
        dispatched = false;

    }

    void PopListenerDispatch() {

        listenerStack.pop();
        dispatched = false;

    }

    virtual void HandleEvent(srcSAXEventDispatch::ParserState pstate, srcSAXEventDispatch::ElementState estate, srcSAXEventDispatch::srcSAXEventContext& ctx) override {

        while(!dispatched) {

            dispatched = true;

            switch(estate) {

                case srcSAXEventDispatch::ElementState::open: {
                    auto event = listenerStack.top()->GetOpenEventMap().find(pstate);
                    if(event != listenerStack.top()->GetOpenEventMap().end()){
                        event->second(ctx);
                    }
                    break;
                }

                case srcSAXEventDispatch::ElementState::close: {
                    auto event = listenerStack.top()->GetCloseEventMap().find(pstate);
                    if(event != listenerStack.top()->GetCloseEventMap().end()){
                        event->second(ctx);
                    }
                    break;
                }

                default:
                    throw std::runtime_error("Something went terribly, terribly wrong");

            }

        }

    }

};

#endif
