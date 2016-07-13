/**
 * @file srcSliceHandler.hpp
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

#ifndef INCLUDED_SRCSAXEVENTDISPATCH_HPP
#define INCLUDED_SRCSAXEVENTDISPATCH_HPP

#include <srcSAXHandler.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
namespace srcSAXEventDispatch {
    enum ParserState {decl, expr, parameter, declstmt, exprstmt, parameterlist, 
        argumentlist, argumentlisttemplate, call, templates, ctrlflow, endflow, 
        name, function, functiondecl, constructor, constructordecl, destructordecl, destructor,
        argument, index, block, type, init, op, literal, modifier, memberlist, classn, structn,
        preproc, whileloop, forloop, ifcond, nonterminal, macro, classblock, functionblock,
        specifier, typedefexpr, empty, MAXENUMVALUE = empty};
    enum ElementState {open, close};
    class Listener{
        public:
            virtual void handleEvent(){}
            virtual void handleEvent(ParserState, ElementState){}
            virtual ~Listener(){}
            Listener(){}
    };
    class EventDispatcher{
    public:
        virtual void addListener( Listener *l ){}
        virtual void removeListener( Listener *l ){}
    protected:
        std::vector<Listener*> mListeners;
        virtual void dispatchEvent(ParserState, ElementState)=0;
        virtual ~EventDispatcher(){}
        EventDispatcher(){}
    };
    class srcSAXEventDispatcher : public srcSAXHandler, public EventDispatcher {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"

    std::vector<unsigned short int> triggerField;
    std::unordered_map< std::string, std::function<void()>> process_map, process_map2;
    protected:
        std::vector<Listener*> mListeners;
        void dispatchEvent(ParserState, ElementState) override;

    public:
        void addListener(Listener* l) override;
        void removeListener(Listener* l) override;
        ~srcSAXEventDispatcher(){}
        srcSAXEventDispatcher(){
                process_map = {
                    {"decl_stmt", [this](){
                        ++triggerField[declstmt];
                        dispatchEvent(declstmt, ElementState::open);
                    } },
                    { "expr_stmt", [this](){
                        ++triggerField[exprstmt];
                    } },
                    { "parameter_list", [this](){
                        ++triggerField[parameterlist];
                    } },
                    { "if", [this](){
                        ++triggerField[ifcond];
                    } },
                    { "for", [this](){
                        ++triggerField[forloop];
                    } },
                    { "while", [this](){
                        ++triggerField[whileloop];
                    } },
                    { "template", [this](){
                        ++triggerField[templates];
                    } },
                    { "argument_list", [this](){
                    } },
                    { "call", [this](){
                        ++triggerField[call];
                    } },
                    { "function", [this](){
                        ++triggerField[function];
                    } },
                    { "constructor", [this](){
                        ++triggerField[constructor];
                    } },
                    { "function_decl", [this](){
                        ++triggerField[functiondecl];
                    } },
                    { "destructor_decl", [this](){
                        ++triggerField[destructordecl];
                    } },
                    { "constructor_decl", [this](){
                        ++triggerField[constructordecl];
                    } },
                    { "class", [this](){
                        ++triggerField[classn];
                    } },
                    { "struct", [this](){
                        ++triggerField[classn];
                    } },
                    { "destructor", [this](){
                        ++triggerField[destructor];
                    } },
                    { "parameter", [this](){
                        ++triggerField[parameter];
                    } },                
                    { "member_list", [this](){
                        ++triggerField[memberlist];
                    } },
                    { "index", [this](){
                        ++triggerField[index];
                    } },
                    { "operator", [this](){
                        ++triggerField[op];
                    } },
                    { "block", [this](){ 
                        ++triggerField[block];
                    } },
                    { "init", [this](){
                        ++triggerField[init];
                    } },
                    { "argument", [this](){
                        ++triggerField[argument];
                    } },
                    { "literal", [this](){
                        ++triggerField[literal];
                    } },
                    { "modifier", [this](){
                        ++triggerField[modifier];
                    } },
                    { "decl", [this](){
                        ++triggerField[decl]; 
                    } },
                    { "type", [this](){
                        ++triggerField[type]; 
                    } },
                    { "typedef", [this](){
                        ++triggerField[typedefexpr]; 
                    } },          
                    { "expr", [this](){
                        ++triggerField[expr];
                    } },
                    { "name", [this](){
                        ++triggerField[name];
                    } },
                    { "macro", [this](){
                        ++triggerField[macro];
                    } },
                    { "specifier", [this](){
                        ++triggerField[specifier];
                    } }
                };
                process_map2 = {
                    {"decl_stmt", [this](){
                        --triggerField[declstmt];
                    } },             
                    { "expr_stmt", [this](){
                        --triggerField[exprstmt];
                    } },            
                    { "parameter_list", [this](){
                        --triggerField[parameterlist];
                    } },            
                    { "if", [this](){
                        --triggerField[ifcond];
                    } },            
                    { "for", [this](){
                        --triggerField[forloop];
                    } },            
                    { "while", [this](){
                        --triggerField[whileloop];
                    } },
                    { "template", [this](){
                        --triggerField[templates];
                    } },            
                    { "argument_list", [this](){
                    } },            
                    { "call", [this](){
                        --triggerField[call];
                    } },            
                    { "function", [this](){
                        --triggerField[function];
                    } },
                    { "constructor", [this](){
                        --triggerField[constructor];
                    } },
                    { "destructor", [this](){
                        --triggerField[destructor];
                    } },
                    { "function_decl", [this](){
                        --triggerField[functiondecl];
                    } },
                    { "constructor_decl", [this](){
                        --triggerField[constructordecl];
                    } },
                    { "destructor_decl", [this](){
                        --triggerField[destructordecl];
                    } },
                    { "class", [this](){
                        --triggerField[classn];
                    } },
                    { "struct", [this](){
                        --triggerField[classn];
                    } },
                    { "parameter", [this](){
                    } },    
                    { "member_list", [this](){
                        --triggerField[memberlist];
                    } },    
                    { "index", [this](){
                        --triggerField[index];
                    } },    
                    { "operator", [this](){
                        --triggerField[op];
                    } },
                    { "block", [this](){ 
                        --triggerField[block];
                    } },
                    { "init", [this](){
                        --triggerField[init];
                    } },    
                    { "argument", [this](){
                        --triggerField[argument];
                    } },    
                    { "literal", [this](){
                        --triggerField[literal];
                    } },    
                    { "modifier", [this](){
                        --triggerField[modifier];
                    } },    
                    { "decl", [this](){

                        --triggerField[decl]; 
                    } },    
                    { "type", [this](){
                        --triggerField[type];
                    } },
                    { "typedef", [this](){
                        --triggerField[typedefexpr]; 
                    } },    
                    { "expr", [this](){
                        --triggerField[expr];
                    } },    
                    { "name", [this](){
                        --triggerField[name];
                    } },
                    { "macro", [this](){
                        --triggerField[macro];
                    } },
                    { "specifier", [this](){
                        --triggerField[specifier];
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
                            const struct srcsax_attribute * attributes) {
            
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
                            const struct srcsax_attribute * attributes) {

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
                                    const struct srcsax_attribute * attributes) {

    
        }
        /**
        * charactersUnit
        * @param ch the characers
        * @param len number of characters
        *
        * SAX handler function for character handling within a unit.
        * Overide for desired behaviour.
        */
        virtual void charactersUnit(const char * ch, int len) {
        }
    
        // end elements may need to be used if you want to collect only on per file basis or some other granularity.
        virtual void endRoot(const char * localname, const char * prefix, const char * URI) {
    
        }
        virtual void endUnit(const char * localname, const char * prefix, const char * URI) {
    
        }
    
        virtual void endElement(const char * localname, const char * prefix, const char * URI) {

        }
    #pragma GCC diagnostic pop
    
    };
}
#endif