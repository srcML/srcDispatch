/**
 * @file srcDispatcherSingleEvent.hpp
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
    class srcDispatcherSingleEvent : public srcDispatcher<policies...> {

    private:
        bool dispatched;

    public:

       srcDispatcherSingleEvent(PolicyListener * listener) : srcDispatcher<policies...>(listener), dispatched(false) {}
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
        virtual void DispatchEvent(srcDispatch::ParserState pstate, srcDispatch::ElementState estate) override {

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
