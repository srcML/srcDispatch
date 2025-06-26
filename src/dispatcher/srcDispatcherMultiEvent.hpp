/**
 * @file srcDispatcherMultiEvent.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef INCLUDED_SRC_DISPATCH_SINGLE_EVENT_HPP
#define INCLUDED_SRC_DISPATCH_SINGLE_EVENT_HPP

#include <srcDispatcher.hpp>

namespace srcDispatch {
    template <typename ...policies>
    class srcDispatcherMultiEvent : public srcDispatcher<policies...> {

    public:

       srcDispatcherMultiEvent(PolicyListener * listener) : srcDispatcher<policies...>(listener), dispatched(false) {}

        void AddListener(EventListener* listener) override {
            elementListeners.push_back(listener);
        }
        void AddListenerDispatch(EventListener* listener) override {
            if(dispatching) {
                listener->HandleEvent(currentPState, currentEState, ctx);
            }
            AddListener(listener);
        }
        void AddListenerNoDispatch(EventListener* listener) override {
            if(dispatching) {
                listener->SetDispatched(true);
            }
            AddListener(listener);
        }
        void RemoveListener(EventListener* listener) override {
            elementListeners.erase(std::find(elementListeners.begin(), elementListeners.end(), listener));
        }
        void RemoveListenerDispatch(EventListener* listener) override {
            if(dispatching) {
                listener->HandleEvent(currentPState, currentEState, ctx);
            }
            RemoveListener(listener);
        }
        void RemoveListenerNoDispatch(EventListener* listener) override {
            if(dispatching) {
                listener->SetDispatched(true);
            }
            RemoveListener(listener);
        }
    protected:

        void DispatchEvent(ParserState pstate, ElementState estate) override {

            dispatching = true;
            currentPState = pstate;
            currentEState = estate;

            for(std::list<EventListener*>::iterator listener = elementListeners.begin(); listener != elementListeners.end(); ++listener ) {
                (*listener)->HandleEvent(pstate, estate, ctx);
            }
            for(std::list<EventListener*>::iterator listener = elementListeners.begin(); listener != elementListeners.end(); ++listener ) {
                (*listener)->SetDispatched(false);
            }

            dispatching = false;

        }

    };

}

#endif
