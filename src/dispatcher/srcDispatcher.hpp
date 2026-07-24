/**
 * @file srcDispatcher.hpp
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

#ifndef INCLUDED_SRCDISPATCHER_HPP
#define INCLUDED_SRCDISPATCHER_HPP

#include <srcSAXHandler.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <srcDispatchUtilities.hpp>
#include <vector>
#include <optional>
#include <string>
#include <unordered_set>
#include <memory>
#include <cstring>

namespace srcDispatch {

    template<typename... policies>
    static std::list<EventListener*> CreateListenersImpl(PolicyListener * policyListener, std::list<EventListener*> & listeners);

    template<typename... policies>
    static std::list<EventListener*> CreateListeners(PolicyListener * policyListener) {
        std::list<EventListener*> listeners;
        return CreateListenersImpl<policies...>(policyListener, listeners);
    }

    template<typename policy, typename... remaining>
    static std::list<EventListener*> CreateListenersHelper(PolicyListener * policyListener, std::list<EventListener*> & listeners);

    template<typename... policies>
    static std::list<EventListener*> CreateListenersImpl(PolicyListener * policyListener, std::list<EventListener*> & listeners) {
        return CreateListenersHelper<policies...>(policyListener, listeners);
    }

    template<typename policy, typename... remaining>
    static std::list<EventListener*> CreateListenersHelper(PolicyListener * policyListener, std::list<EventListener*> & listeners) {
        listeners.emplace_back(new policy({policyListener}));
        return CreateListenersImpl<remaining...>(policyListener, listeners);
    }
    template<>
    std::list<EventListener*> CreateListenersImpl<>(PolicyListener * listener [[maybe_unused]], std::list<EventListener*> & listeners) {
        return listeners;
    }

    const std::string DIFF_URI = "http://www.srcML.org/srcDiff";

    template<typename... policies>
    class srcDispatcher : public srcSAXHandler, public EventDispatcher {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"

    private:
        std::unordered_map< std::string, std::function<void()>> process_map, process_map2;

        std::vector<size_t> elseif_positions;

        bool dispatching;
        bool dispatched;
        bool generateArchive;

        std::size_t numberAllocatedListeners;

        const std::unordered_set<std::string> nameCollectElements{ "class", "struct", "namespace" };
        std::optional<std::string> collectedText;

    protected:

        virtual void DispatchEvent(srcDispatch::ParserState pstate, srcDispatch::ElementState estate) override {

            srcDispatcher<policies...>::currentPState = pstate;
            srcDispatcher<policies...>::currentEState = estate;

            while(!dispatched) {

                dispatched = true;

                EventDispatcher::elementListeners.back()->HandleEvent(pstate, estate, EventDispatcher::ctx);
                EventDispatcher::elementListeners.back()->SetDispatched(false);

            }

            dispatched = false;

        }

        virtual void AddEvent(const std::string & event) {

            std::pair<std::string, std::function<void()>> openEvent(event, [this, event]() {
                    ctx.currentTag = event; 
                    ++ctx.triggerField[ParserState::userdefined];
                    DispatchEvent(ParserState::userdefined, ElementState::open);
                } );
            process_map.insert(openEvent);

            std::pair<std::string, std::function<void()>> closeEvent(event, [this, event]() {
                    ctx.currentTag = event;
                    DispatchEvent(ParserState::userdefined, ElementState::close);
                    --ctx.triggerField[ParserState::userdefined];
                } );

            process_map2.insert(closeEvent);

        }

        virtual void AddEvents(std::initializer_list<std::string> events) {

            for(const std::string & event : events) {
                AddEvent(event);
            }

        }

        virtual void RemoveEvent(const std::string & event) {
            process_map.erase(event);
            process_map2.erase(event);
        }

        virtual void RemoveEvents(std::initializer_list<std::string> events) {

            for(const std::string & event : events) {
                RemoveEvent(event);
            }

        }

    public:
        virtual ~srcDispatcher() {
            for(std::size_t count = 0; count < numberAllocatedListeners; ++count) {
                delete elementListeners.front();
                elementListeners.pop_front();
            }
        }

        srcDispatcher(PolicyListener * listener, bool genArchive = false) 
            : EventDispatcher(element_stack),
              elseif_positions(), dispatching(false), generateArchive(genArchive), collectedText(std::optional<std::string>()) {
    
            elementListeners = CreateListeners<policies...>(listener);
            numberAllocatedListeners = elementListeners.size();
            
            if(genArchive) {
                ctx.archiveBuffer = xmlBufferCreate();
                ctx.writer = xmlNewTextWriterMemory(ctx.archiveBuffer, 0);
            }
            InitializeHandlers();
        }

        srcDispatcher(std::initializer_list<EventListener*> listeners, bool genArchive = false) 
            : EventDispatcher(element_stack),
              elseif_positions(), dispatching(false), generateArchive(genArchive), collectedText(std::optional<std::string>()) {

            elementListeners = listeners;
            numberAllocatedListeners = elementListeners.size();

            if(genArchive) {
                ctx.archiveBuffer = xmlBufferCreate();
                ctx.writer = xmlNewTextWriterMemory(ctx.archiveBuffer, 0);
            }
            InitializeHandlers();
        }
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
        void InitializeHandlers() {
            process_map = {
                {"decl_stmt", [this]() {
                    ++ctx.triggerField[ParserState::declstmt];
                    DispatchEvent(ParserState::declstmt, ElementState::open);
                } },
                { "expr_stmt", [this]() {
                    ++ctx.triggerField[ParserState::exprstmt];
                    DispatchEvent(ParserState::exprstmt, ElementState::open);
                } },
                { "parameter_list", [this]() {
                    ++ctx.triggerField[ParserState::parameterlist];
                    DispatchEvent(ParserState::parameterlist, ElementState::open);
                } },
                { "condition", [this]() {
                    ++ctx.triggerField[ParserState::condition];
                    DispatchEvent(ParserState::condition, ElementState::open);
                } },
                { "switch", [this]() {
                    ++ctx.triggerField[ParserState::switchstmt];
                    DispatchEvent(ParserState::switchstmt, ElementState::open);
                } },
                { "case", [this]() {
                    ++ctx.triggerField[ParserState::switchcase];
                    DispatchEvent(ParserState::switchcase, ElementState::open);
                } },
                { "do", [this]() {
                    ++ctx.triggerField[ParserState::dostmt];
                    DispatchEvent(ParserState::dostmt, ElementState::open);
                } },
                { "incr", [this]() {
                    ++ctx.triggerField[ParserState::incr];
                    DispatchEvent(ParserState::incr, ElementState::open);
                } },
                { "decr", [this]() {
                    ++ctx.triggerField[ParserState::decr];
                    DispatchEvent(ParserState::decr, ElementState::open);
                } },
                { "if_stmt", [this]() {
                    ++ctx.triggerField[ParserState::ifgroup];
                    DispatchEvent(ParserState::ifgroup, ElementState::open);
                } },
                { "if", [this]() {
                    ++ctx.triggerField[ParserState::ifstmt];
                    DispatchEvent(ParserState::ifstmt, ElementState::open);
                } },
                { "elseif", [this]() {
                    ++ctx.triggerField[ParserState::elseif];
                    DispatchEvent(ParserState::elseif, ElementState::open);
                } },
                { "else", [this]() {
                    ++ctx.triggerField[ParserState::elsestmt];
                    DispatchEvent(ParserState::elsestmt, ElementState::open);
                } },
                { "for", [this]() {
                    ++ctx.triggerField[ParserState::forstmt];
                    DispatchEvent(ParserState::forstmt, ElementState::open);
                } },
                { "control", [this]() {
                    ++ctx.triggerField[ParserState::control];
                    DispatchEvent(ParserState::control, ElementState::open);
                } },
                { "while", [this]() {
                    ++ctx.triggerField[ParserState::whilestmt];
                    DispatchEvent(ParserState::whilestmt, ElementState::open);
                } },
                { "template", [this]() {
                    ++ctx.triggerField[ParserState::templates];
                    DispatchEvent(ParserState::templates, ElementState::open);
                } },
                { "argument_list", [this]() {
                    if(!ctx.genericDepth.empty()) {
                        if(ctx.genericDepth.back() == ctx.depth) {
                            ++ctx.triggerField[ParserState::genericargumentlist];
                            DispatchEvent(ParserState::genericargumentlist, ElementState::open);
                        }
                    }
                    DispatchEvent(ParserState::argumentlist, ElementState::open);
                    ++ctx.triggerField[ParserState::argumentlist];
                } },
                { "call", [this]() {
                    ++ctx.triggerField[ParserState::call];
                    DispatchEvent(ParserState::call, ElementState::open);
                } },
                { "function", [this]() {
                    ++ctx.triggerField[ParserState::function];
                    DispatchEvent(ParserState::function, ElementState::open);
                } },
                { "constructor", [this]() {
                    ++ctx.triggerField[ParserState::constructor];
                    DispatchEvent(ParserState::constructor, ElementState::open);
                } },
                { "function_decl", [this]() {
                    ++ctx.triggerField[ParserState::functiondecl];
                    DispatchEvent(ParserState::functiondecl, ElementState::open);
                } },
                { "destructor_decl", [this]() {
                    ++ctx.triggerField[ParserState::destructordecl];
                    DispatchEvent(ParserState::destructordecl, ElementState::open);
                } },
                { "constructor_decl", [this]() {
                    ++ctx.triggerField[ParserState::constructordecl];
                    DispatchEvent(ParserState::constructordecl, ElementState::open);
                } },
                { "class", [this]() {
                    ++ctx.triggerField[ParserState::classn];
                    DispatchEvent(ParserState::classn, ElementState::open);
                } },
                { "struct", [this]() {
                    ++ctx.triggerField[ParserState::classn];
                    DispatchEvent(ParserState::structn, ElementState::open);
                } },
                { "class_decl", [this]() {
                    ++ctx.triggerField[ParserState::classdecl];
                    DispatchEvent(ParserState::classdecl, ElementState::open);
                } },
                { "enum_decl", [this]() {
                    ++ctx.triggerField[ParserState::classdecl];
                    DispatchEvent(ParserState::enumdecl, ElementState::open);
                } },
                { "struct_decl", [this]() {
                    ++ctx.triggerField[ParserState::classdecl];
                    DispatchEvent(ParserState::structdecl, ElementState::open);
                } },
                { "union_decl", [this]() {
                    ++ctx.triggerField[ParserState::classdecl];
                    DispatchEvent(ParserState::uniondecl, ElementState::open);
                } },
                { "namespace", [this]() {
                    ++ctx.triggerField[ParserState::namespacen];
                    DispatchEvent(ParserState::namespacen, ElementState::open);
                } },
                { "super_list", [this]() {
                    ++ctx.triggerField[ParserState::super_list];
                    DispatchEvent(ParserState::super_list, ElementState::open);
                } },
                { "super", [this]() {
                    ++ctx.triggerField[ParserState::super];
                    DispatchEvent(ParserState::super, ElementState::open);
                } },
                { "public", [this]() {
                    ++ctx.triggerField[ParserState::publicaccess];
                    DispatchEvent(ParserState::publicaccess, ElementState::open);
                } },
                { "protected", [this]() {
                    ++ctx.triggerField[ParserState::protectedaccess];
                    DispatchEvent(ParserState::protectedaccess, ElementState::open);
                } },
                { "private", [this]() {
                    ++ctx.triggerField[ParserState::privateaccess];
                    DispatchEvent(ParserState::privateaccess, ElementState::open);
                } },
                { "destructor", [this]() {
                    ++ctx.triggerField[ParserState::destructor];
                    DispatchEvent(ParserState::destructor, ElementState::open);
                } },
                { "parameter", [this]() {
                    ++ctx.triggerField[ParserState::parameter];
                    DispatchEvent(ParserState::parameter, ElementState::open);
                } },                
                { "member_list", [this]() {
                    ++ctx.triggerField[ParserState::memberlist];
                    DispatchEvent(ParserState::memberlist, ElementState::open);
                } },
                { "index", [this]() {
                    ++ctx.triggerField[ParserState::index];
                    DispatchEvent(ParserState::index, ElementState::open);
                } },
                { "operator", [this]() {
                    ++ctx.triggerField[ParserState::op];
                    DispatchEvent(ParserState::op, ElementState::open);
                } },
                { "block", [this]() { 
                    ++ctx.triggerField[ParserState::block];
                    DispatchEvent(ParserState::block, ElementState::open);
                } },
                { "init", [this]() {
                    ++ctx.triggerField[ParserState::init];
                    DispatchEvent(ParserState::init, ElementState::open);
                } },
                { "range", [this]() {
                    ++ctx.triggerField[ParserState::range];
                    DispatchEvent(ParserState::range, ElementState::open);
                } },
                { "argument", [this]() {
                    ++ctx.triggerField[ParserState::argument];
                    DispatchEvent(ParserState::argument, ElementState::open);
                } },
                { "literal", [this]() {
                    ++ctx.triggerField[ParserState::literal];
                    DispatchEvent(ParserState::literal, ElementState::open);
                } },
                { "modifier", [this]() {
                    ++ctx.triggerField[ParserState::modifier];
                    DispatchEvent(ParserState::modifier, ElementState::open);
                } },
                { "decl", [this]() {
                    ++ctx.triggerField[ParserState::decl]; 
                    DispatchEvent(ParserState::decl, ElementState::open);
                } },
                { "type", [this]() {
                    if(ctx.isPrev) {
                        ++ctx.triggerField[ParserState::typeprev]; 
                        DispatchEvent(ParserState::typeprev, ElementState::open);
                    }
                    ++ctx.triggerField[ParserState::type]; 
                    DispatchEvent(ParserState::type, ElementState::open);
                } },
                { "typedef", [this]() {
                    ++ctx.triggerField[ParserState::typedefdecl]; 
                    DispatchEvent(ParserState::typedefdecl, ElementState::open);
                } },          
                { "expr", [this]() {
                    ++ctx.triggerField[ParserState::expr];
                    DispatchEvent(ParserState::expr, ElementState::open);
                } },
                { "name", [this]() {
                    ++ctx.triggerField[ParserState::name];
                    DispatchEvent(ParserState::name, ElementState::open);
                } },
                { "macro", [this]() {
                    ++ctx.triggerField[ParserState::macro];
                    DispatchEvent(ParserState::macro, ElementState::open);
                } },
                { "specifier", [this]() {
                    ++ctx.triggerField[ParserState::specifier];
                    DispatchEvent(ParserState::specifier, ElementState::open);
                } },
                { "noun", [this]() {                    
                    ++ctx.triggerField[ParserState::snoun];
                    DispatchEvent(ParserState::snoun, ElementState::open);
                } },
                { "propernoun", [this]() {
                    ++ctx.triggerField[ParserState::propersnoun];
                    DispatchEvent(ParserState::propersnoun, ElementState::open);
                } },
                { "pronoun", [this]() {
                    ++ctx.triggerField[ParserState::spronoun];
                    DispatchEvent(ParserState::spronoun, ElementState::open);
                } },
                { "adjective", [this]() {
                    ++ctx.triggerField[ParserState::sadjective];
                    DispatchEvent(ParserState::sadjective, ElementState::open);
                } },
                { "verb", [this]() {
                    ++ctx.triggerField[ParserState::sverb];
                    DispatchEvent(ParserState::sverb, ElementState::open);
                } },
                { "stereotype", [this]() {
                    ++ctx.triggerField[ParserState::stereotype];
                    DispatchEvent(ParserState::stereotype, ElementState::open);
                } },
                { "unit", [this]() {
                    if(ctx.triggerField[ParserState::unit] == 0) {
                        ctx.triggerField[ParserState::archive] = 1;
                        DispatchEvent(ParserState::archive, ElementState::open);
                    }
                    ++ctx.triggerField[ParserState::unit];
                    DispatchEvent(ParserState::unit, ElementState::open);
                } },
                { "throws", [this]() {
                    ++ctx.triggerField[ParserState::throws];
                    DispatchEvent(ParserState::throws, ElementState::open);
                } },
                { "throw", [this]() {
                    ++ctx.triggerField[ParserState::throwstmt];
                    DispatchEvent(ParserState::throwstmt, ElementState::open);
                } },
                { "try", [this]() {
                    ++ctx.triggerField[ParserState::trystmt];
                    DispatchEvent(ParserState::trystmt, ElementState::open);
                } },
                { "catch", [this]() {
                    ++ctx.triggerField[ParserState::catchstmt];
                    DispatchEvent(ParserState::catchstmt, ElementState::open);
                } },
                { "annotation", [this]() {
                    ++ctx.triggerField[ParserState::annotation];
                    DispatchEvent(ParserState::annotation, ElementState::open);
                } },
                { "return", [this]() {
                    ++ctx.triggerField[ParserState::returnstmt];
                    DispatchEvent(ParserState::returnstmt, ElementState::open);
                } },
                { "goto", [this]() {
                    ++ctx.triggerField[ParserState::gotostmt];
                    DispatchEvent(ParserState::gotostmt, ElementState::open);
                } },
                { "break", [this]() {
                    ++ctx.triggerField[ParserState::breakstmt];
                    DispatchEvent(ParserState::breakstmt, ElementState::open);
                } },
                { "continue", [this]() {
                    ++ctx.triggerField[ParserState::continuestmt];
                    DispatchEvent(ParserState::continuestmt, ElementState::open);
                } },
                { "label", [this]() {
                    ++ctx.triggerField[ParserState::label];
                    DispatchEvent(ParserState::label, ElementState::open);
                } },
                { "comment", [this]() {
                    ++ctx.triggerField[ParserState::comment];
                    DispatchEvent(ParserState::comment, ElementState::open);
                } },
                { "cpp:include", [this]() {
                    ++ctx.triggerField[ParserState::cppinclude];
                    DispatchEvent(ParserState::cppinclude, ElementState::open);
                } },
                { "cpp:directive", [this]() {
                    ++ctx.triggerField[ParserState::cppdirective];
                    DispatchEvent(ParserState::cppdirective, ElementState::open);
                } },
                { "cpp:file", [this]() {
                    ++ctx.triggerField[ParserState::cppfile];
                    DispatchEvent(ParserState::cppfile, ElementState::open);
                } },
            };
            process_map2 = {
                {"decl_stmt", [this]() {
                    DispatchEvent(ParserState::declstmt, ElementState::close);
                    --ctx.triggerField[ParserState::declstmt];
                } },             
                { "expr_stmt", [this]() {
                    DispatchEvent(ParserState::exprstmt, ElementState::close);
                    --ctx.triggerField[ParserState::exprstmt];
                } },            
                { "parameter_list", [this]() {
                    DispatchEvent(ParserState::parameterlist, ElementState::close);
                    --ctx.triggerField[ParserState::parameterlist];
                } },       
                { "condition", [this]() {
                    DispatchEvent(ParserState::condition, ElementState::close);
                    --ctx.triggerField[ParserState::condition];
                } },
                { "switch", [this]() {
                    DispatchEvent(ParserState::switchstmt, ElementState::close);
                    --ctx.triggerField[ParserState::switchstmt];
                } },
                { "case", [this]() {
                    DispatchEvent(ParserState::switchcase, ElementState::close);
                    --ctx.triggerField[ParserState::switchcase];
                } },
                { "do", [this]() {
                    DispatchEvent(ParserState::dostmt, ElementState::close);
                    --ctx.triggerField[ParserState::dostmt];
                } },
                { "incr", [this]() {
                    DispatchEvent(ParserState::incr, ElementState::close);
                    --ctx.triggerField[ParserState::incr];
                } },
                { "decr", [this]() {
                    DispatchEvent(ParserState::decr, ElementState::close);
                    --ctx.triggerField[ParserState::decr];
                } },
                { "if_stmt", [this]() {
                    DispatchEvent(ParserState::ifgroup, ElementState::close);
                    --ctx.triggerField[ParserState::ifgroup];
                } },
                { "if", [this]() {
                    DispatchEvent(ParserState::ifstmt, ElementState::close);
                    --ctx.triggerField[ParserState::ifstmt];
                } },
                { "elseif", [this]() {
                    DispatchEvent(ParserState::elseif, ElementState::close);
                    --ctx.triggerField[ParserState::elseif];
                } }, 
                { "else", [this]() {
                    --ctx.triggerField[ParserState::elsestmt];
                    DispatchEvent(ParserState::elsestmt, ElementState::close);
                } },
                { "for", [this]() {
                    DispatchEvent(ParserState::forstmt, ElementState::close);
                    --ctx.triggerField[ParserState::forstmt];
                } },  
                { "control", [this]() {
                    --ctx.triggerField[ParserState::control];
                    DispatchEvent(ParserState::control, ElementState::close);
                } },           
                { "while", [this]() {
                    DispatchEvent(ParserState::whilestmt, ElementState::close);
                    --ctx.triggerField[ParserState::whilestmt];
                } },
                { "template", [this]() {
                    DispatchEvent(ParserState::templates, ElementState::close);
                    --ctx.triggerField[ParserState::templates];
                } },            
                { "argument_list", [this]() {
                    if(!ctx.genericDepth.empty()) {
                        if(ctx.genericDepth.back() == ctx.depth) {
                            DispatchEvent(ParserState::genericargumentlist, ElementState::close);
                            --ctx.triggerField[ParserState::genericargumentlist];
                            ctx.genericDepth.pop_back();
                        }
                    }
                    DispatchEvent(ParserState::argumentlist, ElementState::close);
                    --ctx.triggerField[ParserState::argumentlist];
                } },            
                { "call", [this]() {
                    DispatchEvent(ParserState::call, ElementState::close);
                    --ctx.triggerField[ParserState::call];
                } },            
                { "function", [this]() {
                    DispatchEvent(ParserState::function, ElementState::close);
                    --ctx.triggerField[ParserState::function];
                } },
                { "constructor", [this]() {
                    DispatchEvent(ParserState::constructor, ElementState::close);
                    --ctx.triggerField[ParserState::constructor];
                } },
                { "destructor", [this]() {
                    DispatchEvent(ParserState::destructor, ElementState::close);
                    --ctx.triggerField[ParserState::destructor];
                } },
                { "function_decl", [this]() {
                    DispatchEvent(ParserState::functiondecl, ElementState::close);
                    --ctx.triggerField[ParserState::functiondecl];
                } },
                { "constructor_decl", [this]() {
                    DispatchEvent(ParserState::constructordecl, ElementState::close);
                    --ctx.triggerField[ParserState::constructordecl];
                } },
                { "destructor_decl", [this]() {
                    DispatchEvent(ParserState::destructordecl, ElementState::close);
                    --ctx.triggerField[ParserState::destructordecl];
                } },
                { "class", [this]() {
                    ctx.currentClassName.clear();
                    DispatchEvent(ParserState::classn, ElementState::close);
                    --ctx.triggerField[ParserState::classn];
                } },
                { "struct", [this]() {
                    ctx.currentClassName.clear();
                    DispatchEvent(ParserState::structn, ElementState::close);
                    --ctx.triggerField[ParserState::classn];
                } },
                { "class_decl", [this]() {
                    DispatchEvent(ParserState::classdecl, ElementState::close);
                    --ctx.triggerField[ParserState::classdecl];
                } },
                { "enum_decl", [this]() {
                    DispatchEvent(ParserState::classdecl, ElementState::close);
                    --ctx.triggerField[ParserState::enumdecl];
                } },
                { "struct_decl", [this]() {
                    DispatchEvent(ParserState::classdecl, ElementState::close);
                    --ctx.triggerField[ParserState::structdecl];
                } },
                { "union_decl", [this]() {
                    DispatchEvent(ParserState::classdecl, ElementState::close);
                    --ctx.triggerField[ParserState::uniondecl];
                } },
                { "namespace", [this]() {
                    DispatchEvent(ParserState::namespacen, ElementState::close);
                    --ctx.triggerField[ParserState::namespacen];
                } },
                { "super_list", [this]() {
                    DispatchEvent(ParserState::super_list, ElementState::close);
                    --ctx.triggerField[ParserState::super_list];
                } },
                { "super", [this]() {
                    DispatchEvent(ParserState::super, ElementState::close);
                    --ctx.triggerField[ParserState::super];
                } },
                { "public", [this]() {
                    DispatchEvent(ParserState::publicaccess, ElementState::close);
                    --ctx.triggerField[ParserState::publicaccess];
                } },
                { "protected", [this]() {
                    DispatchEvent(ParserState::protectedaccess, ElementState::close);
                    --ctx.triggerField[ParserState::protectedaccess];
                } },
                { "private", [this]() {
                    DispatchEvent(ParserState::privateaccess, ElementState::close);
                    --ctx.triggerField[ParserState::privateaccess];
                } },
                { "parameter", [this]() {
                    DispatchEvent(ParserState::parameter, ElementState::close);
                    --ctx.triggerField[ParserState::parameter];
                } },    
                { "member_list", [this]() {
                    DispatchEvent(ParserState::memberlist, ElementState::close);
                    --ctx.triggerField[ParserState::memberlist];
                } },    
                { "index", [this]() {
                    DispatchEvent(ParserState::index, ElementState::close);
                    --ctx.triggerField[ParserState::index];
                } },    
                { "operator", [this]() {
                    DispatchEvent(ParserState::op, ElementState::close);
                    --ctx.triggerField[ParserState::op];
                } },
                { "block", [this]() { 
                    DispatchEvent(ParserState::block, ElementState::close);
                    --ctx.triggerField[ParserState::block];
                } },
                { "init", [this]() {
                    DispatchEvent(ParserState::init, ElementState::close);
                    --ctx.triggerField[ParserState::init];
                } },
                { "range", [this]() {
                    DispatchEvent(ParserState::range, ElementState::close);
                    --ctx.triggerField[ParserState::range];
                } },
                { "argument", [this]() {
                    DispatchEvent(ParserState::argument, ElementState::close);
                    --ctx.triggerField[ParserState::argument];
                } },    
                { "literal", [this]() {
                    DispatchEvent(ParserState::literal, ElementState::close);
                    --ctx.triggerField[ParserState::literal];
                } },    
                { "modifier", [this]() {
                    DispatchEvent(ParserState::modifier, ElementState::close);
                    --ctx.triggerField[ParserState::modifier];
                } },    
                { "decl", [this]() {
                    DispatchEvent(ParserState::decl, ElementState::close);
                    --ctx.triggerField[ParserState::decl]; 
                } },    
                { "type", [this]() {
                    if(ctx.isPrev) {
                        DispatchEvent(ParserState::typeprev, ElementState::close);
                        --ctx.triggerField[ParserState::typeprev];
                    }
                    DispatchEvent(ParserState::type, ElementState::close);
                    --ctx.triggerField[ParserState::type];
                } },
                { "typedef", [this]() {
                    DispatchEvent(ParserState::typedefdecl, ElementState::close);
                    --ctx.triggerField[ParserState::typedefdecl]; 
                } },    
                { "expr", [this]() {
                    DispatchEvent(ParserState::expr, ElementState::close);
                    --ctx.triggerField[ParserState::expr];
                } },    
                { "name", [this]() {
                    DispatchEvent(ParserState::name, ElementState::close);
                    --ctx.triggerField[ParserState::name];
                } },
                { "macro", [this]() {
                    DispatchEvent(ParserState::macro, ElementState::close);
                    --ctx.triggerField[ParserState::macro];
                } },
                { "specifier", [this]() {
                    DispatchEvent(ParserState::specifier, ElementState::close);
                    --ctx.triggerField[ParserState::specifier];
                } },
                { "noun", [this]() {
                    --ctx.triggerField[ParserState::snoun];
                    DispatchEvent(ParserState::snoun, ElementState::close);
                } },
                { "propernoun", [this]() {
                    --ctx.triggerField[ParserState::propersnoun];
                    DispatchEvent(ParserState::propersnoun, ElementState::close);
                } },
                { "pronoun", [this]() {
                    --ctx.triggerField[ParserState::spronoun];
                    DispatchEvent(ParserState::spronoun, ElementState::close);
                } },
                { "adjective", [this]() {
                    --ctx.triggerField[ParserState::sadjective];
                    DispatchEvent(ParserState::sadjective, ElementState::close);
                } },
                { "verb", [this]() {
                    --ctx.triggerField[ParserState::sverb];
                    DispatchEvent(ParserState::sverb, ElementState::close);
                } },
                { "stereotype", [this]() {
                    DispatchEvent(ParserState::stereotype, ElementState::close);
                    --ctx.triggerField[ParserState::stereotype];
                } },
                { "unit", [this]() {
                    --ctx.triggerField[ParserState::unit];
                    DispatchEvent(ParserState::unit, ElementState::close);
                    if(ctx.triggerField[ParserState::unit] == 0) {
                        ctx.triggerField[ParserState::archive] = 0;
                        DispatchEvent(ParserState::archive, ElementState::close);
                    }
                } },
                { "return", [this]() {
                    --ctx.triggerField[ParserState::returnstmt];
                    DispatchEvent(ParserState::returnstmt, ElementState::close);
                } },
                { "goto", [this]() {
                    --ctx.triggerField[ParserState::gotostmt];
                    DispatchEvent(ParserState::gotostmt, ElementState::close);
                } },
                { "break", [this]() {
                    --ctx.triggerField[ParserState::breakstmt];
                    DispatchEvent(ParserState::breakstmt, ElementState::close);
                } },
                { "continue", [this]() {
                    --ctx.triggerField[ParserState::continuestmt];
                    DispatchEvent(ParserState::continuestmt, ElementState::close);
                } },
                { "label", [this]() {
                    --ctx.triggerField[ParserState::label];
                    DispatchEvent(ParserState::label, ElementState::close);
                } },
                { "throws", [this]() {
                    --ctx.triggerField[ParserState::throws];
                    DispatchEvent(ParserState::throws, ElementState::close);
                } },
                { "throw", [this]() {
                    --ctx.triggerField[ParserState::throwstmt];
                    DispatchEvent(ParserState::throwstmt, ElementState::close);
                } },
                { "try", [this]() {
                    --ctx.triggerField[ParserState::trystmt];
                    DispatchEvent(ParserState::trystmt, ElementState::close);
                } },
                { "catch", [this]() {
                    --ctx.triggerField[ParserState::catchstmt];
                    DispatchEvent(ParserState::catchstmt, ElementState::close);
                } },
                { "annotation", [this]() {
                    --ctx.triggerField[ParserState::annotation];
                    DispatchEvent(ParserState::annotation, ElementState::close);
                } },
                { "comment", [this]() {
                    --ctx.triggerField[ParserState::comment];
                    DispatchEvent(ParserState::comment, ElementState::close);
                } },
                { "cpp:include", [this]() {
                    --ctx.triggerField[ParserState::cppinclude];
                    DispatchEvent(ParserState::cppinclude, ElementState::close);
                } },
                { "cpp:directive", [this]() {
                    --ctx.triggerField[ParserState::cppdirective];
                    DispatchEvent(ParserState::cppdirective, ElementState::close);
                } },
                { "cpp:file", [this]() {
                    --ctx.triggerField[ParserState::cppfile];
                    DispatchEvent(ParserState::cppfile, ElementState::close);
                } },
                { "xmlattribute", [this]() {
                    ctx.triggerField[ParserState::xmlattribute] = 1;
                    DispatchEvent(ParserState::xmlattribute, ElementState::close);
                    ctx.triggerField[ParserState::xmlattribute] = 0;
                } },
                { "tokenstring", [this]() {
                    ctx.triggerField[ParserState::tokenstring] = 1;
                    DispatchEvent(ParserState::tokenstring, ElementState::close);
                    ctx.triggerField[ParserState::tokenstring] = 0;
                } }
            };            
        }

        virtual void startDocument() override {
            if (generateArchive) { xmlTextWriterStartDocument(ctx.writer, "1.0", "UTF-8", "yes"); }
        }
        virtual void endDocument() override {
            if (generateArchive) {
                xmlTextWriterEndDocument(ctx.writer);
                auto buf = GetXmlBuffer();
                fprintf(stdout, "%s", buf->content);
            }
        }
    
        /**
        * startRoot
        * @param localname the name of the element tag
        * @param prefix the tag prefix
        * @param URI the namespace of tag
        * @param nb_namespaces number of namespaces definitions
        * @param namespaces the defined namespaces
        * @param nb_attributes the number of attributes on the tag
        * @param nb_defaulted the number of defaulted attributes
        * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
        *
        * SAX handler function for start of the root element.
        * Counts the root unit (if an archive, to avoid double count with startUnit).
        * Overide for desired behaviour.
        */
        virtual void startRoot(const char * localname, const char * prefix, const char * URI,
                            int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                            const struct srcsax_attribute * attributes) override {
            if(is_archive && generateArchive) {
                ctx.write_start_tag(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);
            }
            std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map.find("unit");
            if (process != process_map.end()) {
                process->second();
            }
        }
        /**
        * startUnit
        * @param localname the name of the element tag
        * @param prefix the tag prefix
        * @param URI the namespace of tag
        * @param nb_namespaces number of namespaces definitions
        * @param namespaces the defined namespaces
        * @param nb_attributes the number of attributes on the tag
        * @param nb_defaulted the number of defaulted attributes
        * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
        *
        * SAX handler function for start of an unit.
        * Counts each unit tag (= filecount non-archive, = filecount + 1 if archive).
        * Overide for desired behaviour.
        */
        virtual void startUnit(const char * localname, const char * prefix, const char * URI,
                            int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                            const struct srcsax_attribute * attributes) override {
            ctx.isArchive = is_archive;
            ++ctx.depth;
            if (generateArchive) {
                ctx.write_start_tag(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);
            }
            std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map.find("unit");
            if (process != process_map.end()) {
                process->second();
            }

            if(num_attributes >= 3) {
                if (num_attributes >= 5)
                    ctx.currentFileChecksum = std::string(attributes[4].value);
                ctx.currentFilePath = std::string(attributes[2].value); 
                ctx.currentFileLanguage = std::string(attributes[1].value);
                ctx.currentsrcMLRevision = std::string(attributes[0].value);
            }
        }

        /**
        * startElementNs
        * @param localname the name of the element tag
        * @param prefix the tag prefix
        * @param URI the namespace of tag
        * @param nb_namespaces number of namespaces definitions
        * @param namespaces the defined namespaces
        * @param nb_attributes the number of attributes on the tag
        * @param nb_defaulted the number of defaulted attributes
        * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
        *
        * SAX handler function for start of an element.
        * Overide for desired behaviour.
        */
        virtual void startElement(const char * localname, const char * prefix, const char * URI,
                                    int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                    const struct srcsax_attribute * attributes) override {
            
            if(generateArchive) {
                ctx.write_start_tag(localname, prefix, URI, num_namespaces, namespaces, num_attributes, attributes);
            }

            if(URI == DIFF_URI) {
                if(localname == std::string("ws")) return;

                static std::unordered_map<std::string, DiffOperation> diff_op_map = { 
                    { "delete", DiffOperation::DELETE },
                    { "insert", DiffOperation::INSERT },
                    { "common", DiffOperation::COMMON },
                };

                bool isReplace = false;
                bool isConvert = false;
                if(num_attributes) {
                    isReplace = attributes[0].value == std::string("replace");
                    isConvert = attributes[0].value == std::string("convert");

                }

                ctx.diffStack.emplace_back(diff_op_map[std::string(localname)], ctx.depth + 1, isReplace, isConvert);


                return;
            }
            
            ++ctx.depth;

            std::string localName = srcSAXHandler::get_qualified_name(localname, prefix);
            ctx.currentTag = localName;

            // Re-think this.  At least use processed list later and think about having a special object.
            std::string name;
            if(num_attributes) {
                name = attributes[0].value;
            }
            if(name == "generic" && localName == "argument_list") {
                ctx.genericDepth.push_back(ctx.depth);
            }
            if(name == "prev" && localName == "type") {
                ctx.isPrev = true;
            }
            if(name == "operator" && (localName == "function" || localName == "function_decl")) {
                ctx.isOperator = true;
            }
            if(name == "pseudo" && localName == "block") {
                ctx.isPseudo = true;
            }

            if(name == "elseif" && localName == "if") {
                localName = "elseif";
                elseif_positions.push_back(element_stack.size());
            }

            if(localName != "") {

                // form attribute map
                for(int pos = 0; pos < num_attributes; ++pos) {

                    std::string attributeName = srcSAXHandler::get_qualified_name(attributes[pos].localname, attributes[pos].prefix);
                    if(strcmp(attributes[pos].localname, "start") == 0) {
                        ctx.startPosition = DeltaElement(Position(attributes[pos].value));
                    } else if(strcmp(attributes[pos].localname, "end") == 0) {
                        ctx.endPosition   = DeltaElement(Position(attributes[pos].value));
                    }

                    std::string attributeValue = attributes[pos].value;
                    ctx.attributes.emplace(attributeName, attributeValue);         

                }

                std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map.find(localName);
                if (process != process_map.end()) {
                    process->second();
                }
                // erase attribute now that call is over
                ctx.attributes.clear();
            }

            if(ctx.currentTag == "namespace") {
                ctx.currentNamespaces.emplace_back();
            }

            if(ctx.currentTag == "name" && nameCollectElements.contains(element_stack.back())) {
                collectedText = std::string();
            } else if(collectedText && (ctx.currentTag == "block" || ctx.currentTag == "super_list")) {
                if(element_stack.back() == "namespace") {
                    ctx.currentNamespaces.back() = *collectedText;
                } else {
                    ctx.currentClassName = *collectedText;
                }
                collectedText = std::optional<std::string>();
            }

            for(int pos = 0; pos < num_attributes; ++pos) {

                ctx.currentAttributeName = "";
                if(attributes[pos].prefix) {
                    ctx.currentAttributeName += attributes[pos].prefix;
                    ctx.currentAttributeName += ':';
                }
                ctx.currentAttributeName += attributes[pos].localname;
                ctx.currentAttributeValue = attributes[pos].value;

                std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map2.find("xmlattribute");
                process->second();                

            }

            ctx.isPrev = false;
            ctx.isOperator = false;
        }
        /**
        * charactersUnit
        * @param ch the characers
        * @param len number of characters
        *
        * SAX handler function for character handling within a unit.
        * Overide for desired behaviour.
        */
        virtual void charactersUnit(const char * ch, int len) override {
            ctx.currentToken.clear();
            ctx.currentToken.append(ch, len);
            std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map2.find("tokenstring");

            if(collectedText) {
        		collectedText->append(ch, len);
            }
            
            process->second();
            if (generateArchive) { ctx.write_content(ctx.currentToken); }
        }
    
        // end elements may need to be used if you want to collect only on per file basis or some other granularity.
        virtual void endRoot(const char * localname, const char * prefix, const char * URI) override {
            std::unordered_map<std::string, std::function<void()>>::const_iterator process2 = process_map2.find("unit");
            if (process2 != process_map2.end()) {
                process2->second();
            }
            if(is_archive && generateArchive) {
                xmlTextWriterEndElement(ctx.writer);
            }            
        }
        virtual void endUnit(const char * localname, const char * prefix, const char * URI) override {
            std::unordered_map<std::string, std::function<void()>>::const_iterator process2 = process_map2.find("unit");
            if (process2 != process_map2.end()) {
                process2->second();
            }

            if (generateArchive) { xmlTextWriterEndElement(ctx.writer); }
            --ctx.depth;

        }
    
        virtual void endElement(const char * localname, const char * prefix, const char * URI) override {

            if(URI == DIFF_URI) {
                if(localname == std::string("ws")) return;

                ctx.diffStack.pop_back();
                return;
            }

            std::string localName =  srcSAXHandler::get_qualified_name(localname, prefix);

            if(!elseif_positions.empty() && element_stack.size() == elseif_positions.back()) {
                localName = "elseif";
                elseif_positions.pop_back();
            }

            ctx.currentTag = localName;

            std::unordered_map<std::string, std::function<void()>>::const_iterator process2 = process_map2.find(localName);
            if (process2 != process_map2.end()) {
                process2->second();
            }

    	    if(ctx.currentTag == "namespace") {
                collectedText = std::optional<std::string>();
	            ctx.currentNamespaces.pop_back();
	        }
	    
            --ctx.depth;

            if (generateArchive) { xmlTextWriterEndElement(ctx.writer); }
        }
    #pragma GCC diagnostic pop
    
    };
}
#endif
