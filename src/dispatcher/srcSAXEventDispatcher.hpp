/**
 * @file srcSAXEventDispatcher.hpp
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


#ifndef INCLUDED_SRCSAXEVENTDISPATCHER_HPP
#define INCLUDED_SRCSAXEVENTDISPATCHER_HPP

#include <srcSAXHandler.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <srcSAXEventDispatchUtilities.hpp>
#include <vector>
#include <memory>
namespace srcSAXEventDispatch {

    template<typename policy, typename... remaining>
    std::list<EventListener*> create_listeners(PolicyListener * listener) {
        std::list<EventListener*> listeners{new policy{listener}};
        std::list<EventListener*> remaining_listeners = create_listeners<remaining...>(listener);
        listeners.insert(listeners.end(), remaining_listeners.begin(), remaining_listeners.end());
        return listeners;
    }
    template<>
    std::list<EventListener*> create_listeners<void>(PolicyListener * listener) {
        return std::list<EventListener*>();
    }

    template <typename ...policies>
    class srcSAXEventDispatcher : public srcSAXHandler, public EventDispatcher {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"

    private:
        std::unordered_map< std::string, std::function<void()>> process_map, process_map2;
        bool classflagopen, functionflagopen, whileflagopen, ifflagopen, elseflagopen, ifelseflagopen, forflagopen, switchflagopen;

        bool dispatching;
        ParserState currentPState;
        ElementState currentEState;

    protected:
        void DispatchEvent(ParserState pstate, ElementState estate) override {

            dispatching = true;
            currentPState = pstate;
            currentEState = estate;

            for(std::list<EventListener*>::iterator listener = elementListeners.begin(); listener != elementListeners.end(); ++listener ){
                (*listener)->HandleEvent(pstate, estate, ctx);
            }
            for(std::list<EventListener*>::iterator listener = elementListeners.begin(); listener != elementListeners.end(); ++listener ){
                (*listener)->SetDispatched(false);
            }

            dispatching = false;

        }

        // struct policy_pack {
        //     std::list<EventListener*> create_listeners(PolicyListener * listener) {
        //         return std::list<EventListener*>{new policy{listener}};
        //     }
        // };
        // struct policy_pack {

    public:
        ~srcSAXEventDispatcher() {}
        srcSAXEventDispatcher(PolicyListener * listener) : EventDispatcher(srcml_element_stack) {
            // policy_pack<policies...> pack;
            elementListeners = create_listeners<policies..., void>(listener);
            std::cerr << "HERE: " << __FILE__ << ' ' << __FUNCTION__ << ' ' << __LINE__ << ' ' << elementListeners.size() << '\n';
            dispatching = false;
            classflagopen = functionflagopen = whileflagopen = ifflagopen = elseflagopen = ifelseflagopen = forflagopen = switchflagopen = false;
            InitializeHandlers();
        }

        void AddListener(EventListener* listener) override {
            elementListeners.push_back(listener);
        }
        void AddListenerDispatch(EventListener* listener) override {
            if(dispatching)
                listener->HandleEvent(currentPState, currentEState, ctx);
            AddListener(listener);
        }
        void AddListenerNoDispatch(EventListener* listener) override {
            if(dispatching)
                listener->SetDispatched(true);
            AddListener(listener);
        }
        void RemoveListener(EventListener* listener) override {
            elementListeners.erase(std::find(elementListeners.begin(), elementListeners.end(), listener));
        }
        void RemoveListenerDispatch(EventListener* listener) override {
            if(dispatching)
                listener->HandleEvent(currentPState, currentEState, ctx);
            RemoveListener(listener);
        }
        void RemoveListenerNoDispatch(EventListener* listener) override {
            if(dispatching)
                listener->SetDispatched(true);
            RemoveListener(listener);
        }
        void InitializeHandlers(){
            process_map = {
                {"decl_stmt", [this](){
                    ++ctx.triggerField[ParserState::declstmt];
                    DispatchEvent(ParserState::declstmt, ElementState::open);
                } },
                { "expr_stmt", [this](){
                    ++ctx.triggerField[ParserState::exprstmt];
                    DispatchEvent(ParserState::exprstmt, ElementState::open);
                } },
                { "parameter_list", [this](){
                    ++ctx.triggerField[ParserState::parameterlist];
                    DispatchEvent(ParserState::parameterlist, ElementState::open);
                } },
                { "if", [this](){
                    ifflagopen = true;
                    ++ctx.triggerField[ParserState::ifstmt];
                    DispatchEvent(ParserState::ifstmt, ElementState::open);
                } },
                { "for", [this](){
                    ++ctx.triggerField[ParserState::forstmt];
                    DispatchEvent(ParserState::forstmt, ElementState::open);
                } },
                { "while", [this](){
                    whileflagopen = true;
                    ++ctx.triggerField[ParserState::whilestmt];
                    DispatchEvent(ParserState::whilestmt, ElementState::open);
                } },
                { "template", [this](){
                    ++ctx.triggerField[ParserState::templates];
                    DispatchEvent(ParserState::templates, ElementState::open);
                } },
                { "argument_list", [this](){
                    if(!ctx.genericDepth.empty()){
                        if(ctx.genericDepth.back() == ctx.depth){
                            ++ctx.triggerField[ParserState::genericargumentlist];
                            DispatchEvent(ParserState::genericargumentlist, ElementState::open);
                        }
                    }
                    DispatchEvent(ParserState::argumentlist, ElementState::open);
                    ++ctx.triggerField[ParserState::argumentlist];
                } },
                { "call", [this](){
                    ++ctx.triggerField[ParserState::call];
                    DispatchEvent(ParserState::call, ElementState::open);
                } },
                { "function", [this](){
                    functionflagopen = true;
                    ++ctx.triggerField[ParserState::function];
                    DispatchEvent(ParserState::function, ElementState::open);
                } },
                { "constructor", [this](){
                    functionflagopen = true;
                    ++ctx.triggerField[ParserState::constructor];
                    DispatchEvent(ParserState::constructor, ElementState::open);
                } },
                { "function_decl", [this](){
                    ++ctx.triggerField[ParserState::functiondecl];
                    DispatchEvent(ParserState::functiondecl, ElementState::open);
                } },
                { "destructor_decl", [this](){
                    ++ctx.triggerField[ParserState::destructordecl];
                    DispatchEvent(ParserState::destructordecl, ElementState::open);
                } },
                { "constructor_decl", [this](){
                    ++ctx.triggerField[ParserState::constructordecl];
                    DispatchEvent(ParserState::constructordecl, ElementState::open);
                } },
                { "class", [this](){
                    classflagopen = true;
                    ++ctx.triggerField[ParserState::classn];
                    DispatchEvent(ParserState::classn, ElementState::open);
                } },
                { "struct", [this](){
                    classflagopen = true;
                    ++ctx.triggerField[ParserState::classn];
                    DispatchEvent(ParserState::structn, ElementState::open);
                } },
                { "super_list", [this](){
                    ++ctx.triggerField[ParserState::super_list];
                    DispatchEvent(ParserState::super_list, ElementState::open);
                } },
                { "super", [this](){
                    ++ctx.triggerField[ParserState::super];
                    DispatchEvent(ParserState::super, ElementState::open);
                } },
                { "public", [this](){
                    ++ctx.triggerField[ParserState::publicaccess];
                    DispatchEvent(ParserState::publicaccess, ElementState::open);
                } },
                { "protected", [this](){
                    ++ctx.triggerField[ParserState::protectedaccess];
                    DispatchEvent(ParserState::protectedaccess, ElementState::open);
                } },
                { "private", [this](){
                    ++ctx.triggerField[ParserState::privateaccess];
                    DispatchEvent(ParserState::privateaccess, ElementState::open);
                } },
                { "destructor", [this](){
                    functionflagopen = true;
                    ++ctx.triggerField[ParserState::destructor];
                    DispatchEvent(ParserState::destructor, ElementState::open);
                } },
                { "parameter", [this](){
                    ++ctx.triggerField[ParserState::parameter];
                    DispatchEvent(ParserState::parameter, ElementState::open);
                } },                
                { "member_list", [this](){
                    ++ctx.triggerField[ParserState::memberlist];
                    DispatchEvent(ParserState::memberlist, ElementState::open);
                } },
                { "index", [this](){
                    ++ctx.triggerField[ParserState::index];
                    DispatchEvent(ParserState::index, ElementState::open);
                } },
                { "operator", [this](){
                    ++ctx.triggerField[ParserState::op];
                    DispatchEvent(ParserState::op, ElementState::open);
                } },
                { "block", [this](){ 
                    ++ctx.triggerField[ParserState::block];
                    if(functionflagopen){
                        functionflagopen = false;
                        ++ctx.triggerField[ParserState::functionblock];
                        DispatchEvent(ParserState::functionblock, ElementState::open);
                    }
                    if(classflagopen){
                        classflagopen = false; //next time it's set to true, we definitely are in a new one.
                        ++ctx.triggerField[ParserState::classblock];
                    }
                    if(whileflagopen){
                        whileflagopen = false;
                        ++ctx.triggerField[ParserState::whileblock];
                    }
                    if(ifelseflagopen){
                        ifflagopen = false;
                        ++ctx.triggerField[ParserState::ifblock];
                    }
                    if(forflagopen){
                        forflagopen = false;
                        ++ctx.triggerField[ParserState::forblock];
                    }
                    DispatchEvent(ParserState::block, ElementState::open);
                } },
                { "init", [this](){
                    ++ctx.triggerField[ParserState::init];
                    DispatchEvent(ParserState::init, ElementState::open);
                } },
                { "argument", [this](){
                    ++ctx.triggerField[ParserState::argument];
                    DispatchEvent(ParserState::argument, ElementState::open);
                } },
                { "literal", [this](){
                    ++ctx.triggerField[ParserState::literal];
                    DispatchEvent(ParserState::literal, ElementState::open);
                } },
                { "modifier", [this](){
                    ++ctx.triggerField[ParserState::modifier];
                    DispatchEvent(ParserState::modifier, ElementState::open);
                } },
                { "decl", [this](){
                    ++ctx.triggerField[ParserState::decl]; 
                    DispatchEvent(ParserState::decl, ElementState::open);
                } },
                { "type", [this](){
                    ++ctx.triggerField[ParserState::type]; 
                    DispatchEvent(ParserState::type, ElementState::open);
                } },
                { "typedef", [this](){
                    ++ctx.triggerField[ParserState::typedefexpr]; 
                    DispatchEvent(ParserState::typedefexpr, ElementState::open);
                } },          
                { "expr", [this](){
                    ++ctx.triggerField[ParserState::expr];
                    DispatchEvent(ParserState::expr, ElementState::open);
                } },
                { "name", [this](){
                    ++ctx.triggerField[ParserState::name];
                    DispatchEvent(ParserState::name, ElementState::open);
                } },
                { "macro", [this](){
                    ++ctx.triggerField[ParserState::macro];
                    DispatchEvent(ParserState::macro, ElementState::open);
                } },
                { "specifier", [this](){
                    ++ctx.triggerField[ParserState::specifier];
                    DispatchEvent(ParserState::specifier, ElementState::open);
                } }
            };
            process_map2 = {
                {"decl_stmt", [this](){
                    DispatchEvent(ParserState::declstmt, ElementState::close);
                    --ctx.triggerField[ParserState::declstmt];
                } },             
                { "expr_stmt", [this](){
                    DispatchEvent(ParserState::exprstmt, ElementState::close);
                    --ctx.triggerField[ParserState::exprstmt];
                } },            
                { "parameter_list", [this](){
                    DispatchEvent(ParserState::parameterlist, ElementState::close);
                    --ctx.triggerField[ParserState::parameterlist];
                } },            
                { "if", [this](){
                    --ctx.triggerField[ParserState::ifblock];
                    DispatchEvent(ParserState::ifstmt, ElementState::close);
                    --ctx.triggerField[ParserState::ifstmt];
                } },            
                { "for", [this](){
                    --ctx.triggerField[ParserState::forblock];
                    DispatchEvent(ParserState::forstmt, ElementState::close);
                    --ctx.triggerField[ParserState::forstmt];
                } },            
                { "while", [this](){
                    --ctx.triggerField[ParserState::whileblock];
                    DispatchEvent(ParserState::whilestmt, ElementState::close);
                    --ctx.triggerField[ParserState::whilestmt];
                } },
                { "template", [this](){
                    DispatchEvent(ParserState::templates, ElementState::close);
                    --ctx.triggerField[ParserState::templates];
                } },            
                { "argument_list", [this](){
                    if(!ctx.genericDepth.empty()){
                        if(ctx.genericDepth.back() == ctx.depth){
                            DispatchEvent(ParserState::genericargumentlist, ElementState::close);
                            --ctx.triggerField[ParserState::genericargumentlist];
                            ctx.genericDepth.pop_back();
                        }
                    }
                    DispatchEvent(ParserState::argumentlist, ElementState::close);
                    --ctx.triggerField[ParserState::argumentlist];
                } },            
                { "call", [this](){
                    DispatchEvent(ParserState::call, ElementState::close);
                    --ctx.triggerField[ParserState::call];
                } },            
                { "function", [this](){
                    DispatchEvent(ParserState::functionblock, ElementState::close);
                    --ctx.triggerField[ParserState::functionblock];

                    DispatchEvent(ParserState::function, ElementState::close);
                    --ctx.triggerField[ParserState::function];
                } },
                { "constructor", [this](){
                    DispatchEvent(ParserState::functionblock, ElementState::close);
                    --ctx.triggerField[ParserState::functionblock];

                    DispatchEvent(ParserState::constructor, ElementState::close);
                    --ctx.triggerField[ParserState::constructor];
                } },
                { "destructor", [this](){
                    DispatchEvent(ParserState::functionblock, ElementState::close);
                    --ctx.triggerField[ParserState::functionblock];
                    
                    DispatchEvent(ParserState::destructor, ElementState::close);
                    --ctx.triggerField[ParserState::destructor];
                } },
                { "function_decl", [this](){
                    DispatchEvent(ParserState::functiondecl, ElementState::close);
                    --ctx.triggerField[ParserState::functiondecl];
                } },
                { "constructor_decl", [this](){
                    DispatchEvent(ParserState::constructordecl, ElementState::close);
                    --ctx.triggerField[ParserState::constructordecl];
                } },
                { "destructor_decl", [this](){
                    DispatchEvent(ParserState::destructordecl, ElementState::close);
                    --ctx.triggerField[ParserState::destructordecl];
                } },
                { "class", [this](){
                    --ctx.triggerField[ParserState::classblock];
                    DispatchEvent(ParserState::classn, ElementState::close);
                    --ctx.triggerField[ParserState::classn];
                } },
                { "struct", [this](){
                    DispatchEvent(ParserState::structn, ElementState::close);
                    --ctx.triggerField[ParserState::classn];
                } },
                { "super_list", [this](){
                    DispatchEvent(ParserState::super_list, ElementState::close);
                    --ctx.triggerField[ParserState::super_list];
                } },
                { "super", [this](){
                    DispatchEvent(ParserState::super, ElementState::close);
                    --ctx.triggerField[ParserState::super];
                } },
                { "public", [this](){
                    DispatchEvent(ParserState::publicaccess, ElementState::close);
                    --ctx.triggerField[ParserState::publicaccess];
                } },
                { "protected", [this](){
                    DispatchEvent(ParserState::protectedaccess, ElementState::close);
                    --ctx.triggerField[ParserState::protectedaccess];
                } },
                { "private", [this](){
                    DispatchEvent(ParserState::privateaccess, ElementState::close);
                    --ctx.triggerField[ParserState::privateaccess];
                } },
                { "parameter", [this](){
                    DispatchEvent(ParserState::parameter, ElementState::close);
                    --ctx.triggerField[ParserState::parameter];
                } },    
                { "member_list", [this](){
                    DispatchEvent(ParserState::memberlist, ElementState::close);
                    --ctx.triggerField[ParserState::memberlist];
                } },    
                { "index", [this](){
                    DispatchEvent(ParserState::index, ElementState::close);
                    --ctx.triggerField[ParserState::index];
                } },    
                { "operator", [this](){
                    DispatchEvent(ParserState::op, ElementState::close);
                    --ctx.triggerField[ParserState::op];
                } },
                { "block", [this](){ 
                    DispatchEvent(ParserState::block, ElementState::close);
                    --ctx.triggerField[ParserState::block];
                } },
                { "init", [this](){
                    DispatchEvent(ParserState::init, ElementState::close);
                    --ctx.triggerField[ParserState::init];
                } },    
                { "argument", [this](){
                    DispatchEvent(ParserState::argument, ElementState::close);
                    --ctx.triggerField[ParserState::argument];
                } },    
                { "literal", [this](){
                    DispatchEvent(ParserState::literal, ElementState::close);
                    --ctx.triggerField[ParserState::literal];
                } },    
                { "modifier", [this](){
                    DispatchEvent(ParserState::modifier, ElementState::close);
                    --ctx.triggerField[ParserState::modifier];
                } },    
                { "decl", [this](){
                    DispatchEvent(ParserState::decl, ElementState::close);
                    --ctx.triggerField[ParserState::decl]; 
                } },    
                { "type", [this](){
                    DispatchEvent(ParserState::type, ElementState::close);
                    --ctx.triggerField[ParserState::type];
                } },
                { "typedef", [this](){
                    DispatchEvent(ParserState::typedefexpr, ElementState::close);
                    --ctx.triggerField[ParserState::typedefexpr]; 
                } },    
                { "expr", [this](){
                    DispatchEvent(ParserState::expr, ElementState::close);
                    --ctx.triggerField[ParserState::expr];
                } },    
                { "name", [this](){
                    DispatchEvent(ParserState::name, ElementState::close);
                    --ctx.triggerField[ParserState::name];
                } },
                { "macro", [this](){
                    DispatchEvent(ParserState::macro, ElementState::close);
                    --ctx.triggerField[ParserState::macro];
                } },
                { "tokenstring", [this](){
                    ctx.triggerField[ParserState::tokenstring] = 1;
                    DispatchEvent(ParserState::tokenstring, ElementState::close);
                    ctx.triggerField[ParserState::tokenstring] = 0;
                } },
                { "specifier", [this](){
                    DispatchEvent(ParserState::specifier, ElementState::close);
                    --ctx.triggerField[ParserState::specifier];
                } }
            };            
        }

        /*
        virtual void startDocument() {}
        virtual void endDocument() {}
        */
    
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
            if(num_attributes >= 3){
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

            ++ctx.depth;

            std::string localName;
            if(prefix) {
                localName += prefix;
                localName += ':';
            }
            localName += localname;

            if(localName == "pos:position"){
                ctx.currentLineNumber = strtoul(attributes[0].value, NULL, 0);
            }
            std::string name;
            if(num_attributes){
                name = attributes[0].value;
            }
            if(name == "generic" && localName == "argument_list"){
                ctx.genericDepth.push_back(ctx.depth);
            }

            if(name == "operator" && (localName == "function" || localName == "function_decl")) {
                ctx.isOperator = true;
            }

            if(localName != ""){
                std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map.find(localname);
                if (process != process_map.end()) {
                    process->second();
                }
            }

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
            process->second();
        }
    
        // end elements may need to be used if you want to collect only on per file basis or some other granularity.
        virtual void endRoot(const char * localname, const char * prefix, const char * URI) override {
    
        }
        virtual void endUnit(const char * localname, const char * prefix, const char * URI) override {
    
        }
    
        virtual void endElement(const char * localname, const char * prefix, const char * URI) override {

            std::string localName;
            if(prefix) {
                localName += prefix;
                localName += ':';
            }
            localName += localname;

            std::unordered_map<std::string, std::function<void()>>::const_iterator process2 = process_map2.find(localName);
            if (process2 != process_map2.end()) {
                process2->second();
            }

            --ctx.depth;

        }
    #pragma GCC diagnostic pop
    
    };
}
#endif