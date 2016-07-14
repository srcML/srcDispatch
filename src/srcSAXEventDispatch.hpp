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
        preproc, whilestmt, forstmt, ifstmt, nonterminal, macro, classblock, functionblock,
        specifier, typedefexpr, empty, MAXENUMVALUE = empty};
    enum ElementState {open, close};
    class Listener{
        public:
            virtual void HandleEvent() = 0;
            virtual void HandleEvent(ParserState, ElementState, std::vector<unsigned short int>) = 0;
    };
    class EventDispatcher{
    public:
        virtual void AddListener(Listener *l) = 0;
        virtual void RemoveListener(Listener *l) = 0;
    protected:
        std::vector<Listener*> mListeners;
        virtual void DispatchEvent(ParserState, ElementState, std::vector<unsigned short int>) = 0;
    };
    class srcSAXEventDispatcher : public srcSAXHandler, public EventDispatcher {
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"

    std::unordered_map< std::string, std::function<void()>> process_map, process_map2;
    protected:
        std::vector<unsigned short int> triggerField;
        std::vector<Listener*> mListeners;
       
        unsigned int currentLineNumber;
        std::string currentFilePath, currentFileName, currentFileLanguage, currentsrcMLRevision;

        void DispatchEvent(ParserState, ElementState, std::vector<unsigned short int>) override;

    public:
        void AddListener(Listener* l) override;
        void RemoveListener(Listener* l) override;
        ~srcSAXEventDispatcher() {}
        srcSAXEventDispatcher(){
                triggerField = std::vector<unsigned short int>(MAXENUMVALUE, 0);
                process_map = {
                    {"decl_stmt", [this](){
                        ++triggerField[declstmt];
                        DispatchEvent(declstmt, ElementState::open, triggerField);
                    } },
                    { "expr_stmt", [this](){
                        ++triggerField[exprstmt];
                        DispatchEvent(exprstmt, ElementState::open, triggerField);
                    } },
                    { "parameter_list", [this](){
                        ++triggerField[parameterlist];
                        DispatchEvent(parameterlist, ElementState::open, triggerField);
                    } },
                    { "if", [this](){
                        ++triggerField[ifstmt];
                        DispatchEvent(ifstmt, ElementState::open, triggerField);
                    } },
                    { "for", [this](){
                        ++triggerField[forstmt];
                        DispatchEvent(forstmt, ElementState::open, triggerField);
                    } },
                    { "while", [this](){
                        ++triggerField[whilestmt];
                        DispatchEvent(whilestmt, ElementState::open, triggerField);
                    } },
                    { "template", [this](){
                        ++triggerField[templates];
                        DispatchEvent(templates, ElementState::open, triggerField);
                    } },
                    { "argument_list", [this](){
                        ++triggerField[argumentlist];
                        DispatchEvent(argumentlist, ElementState::open, triggerField);
                    } },
                    { "call", [this](){
                        ++triggerField[call];
                        DispatchEvent(call, ElementState::open, triggerField);
                    } },
                    { "function", [this](){
                        ++triggerField[function];
                        DispatchEvent(function, ElementState::open, triggerField);
                    } },
                    { "constructor", [this](){
                        ++triggerField[constructor];
                        DispatchEvent(constructor, ElementState::open, triggerField);
                    } },
                    { "function_decl", [this](){
                        ++triggerField[functiondecl];
                        DispatchEvent(functiondecl, ElementState::open, triggerField);
                    } },
                    { "destructor_decl", [this](){
                        ++triggerField[destructordecl];
                        DispatchEvent(destructordecl, ElementState::open, triggerField);
                    } },
                    { "constructor_decl", [this](){
                        ++triggerField[constructordecl];
                        DispatchEvent(constructordecl, ElementState::open, triggerField);
                    } },
                    { "class", [this](){
                        ++triggerField[classn];
                        DispatchEvent(classn, ElementState::open, triggerField);
                    } },
                    { "struct", [this](){
                        ++triggerField[classn];
                        DispatchEvent(structn, ElementState::open, triggerField);
                    } },
                    { "destructor", [this](){
                        ++triggerField[destructor];
                        DispatchEvent(destructor, ElementState::open, triggerField);
                    } },
                    { "parameter", [this](){
                        ++triggerField[parameter];
                        DispatchEvent(parameter, ElementState::open, triggerField);
                    } },                
                    { "member_list", [this](){
                        ++triggerField[memberlist];
                        DispatchEvent(memberlist, ElementState::open, triggerField);
                    } },
                    { "index", [this](){
                        ++triggerField[index];
                        DispatchEvent(index, ElementState::open, triggerField);
                    } },
                    { "operator", [this](){
                        ++triggerField[op];
                        DispatchEvent(op, ElementState::open, triggerField);
                    } },
                    { "block", [this](){ 
                        ++triggerField[block];
                        DispatchEvent(block, ElementState::open, triggerField);
                    } },
                    { "init", [this](){
                        ++triggerField[init];
                        DispatchEvent(init, ElementState::open, triggerField);
                    } },
                    { "argument", [this](){
                        ++triggerField[argument];
                        DispatchEvent(argument, ElementState::open, triggerField);
                    } },
                    { "literal", [this](){
                        ++triggerField[literal];
                        DispatchEvent(literal, ElementState::open, triggerField);
                    } },
                    { "modifier", [this](){
                        ++triggerField[modifier];
                        DispatchEvent(modifier, ElementState::open, triggerField);
                    } },
                    { "decl", [this](){
                        ++triggerField[decl]; 
                        DispatchEvent(decl, ElementState::open, triggerField);
                    } },
                    { "type", [this](){
                        ++triggerField[type]; 
                        DispatchEvent(type, ElementState::open, triggerField);
                    } },
                    { "typedef", [this](){
                        ++triggerField[typedefexpr]; 
                        DispatchEvent(typedefexpr, ElementState::open, triggerField);
                    } },          
                    { "expr", [this](){
                        ++triggerField[expr];
                        DispatchEvent(expr, ElementState::open, triggerField);
                    } },
                    { "name", [this](){
                        ++triggerField[name];
                        DispatchEvent(name, ElementState::open, triggerField);
                    } },
                    { "macro", [this](){
                        ++triggerField[macro];
                        DispatchEvent(macro, ElementState::open, triggerField);
                    } },
                    { "specifier", [this](){
                        ++triggerField[specifier];
                        DispatchEvent(specifier, ElementState::open, triggerField);
                    } }
                };
                process_map2 = {
                    {"decl_stmt", [this](){
                        --triggerField[declstmt];
                        DispatchEvent(declstmt, ElementState::close, triggerField);
                    } },             
                    { "expr_stmt", [this](){
                        --triggerField[exprstmt];
                        DispatchEvent(exprstmt, ElementState::close, triggerField);
                    } },            
                    { "parameter_list", [this](){
                        --triggerField[parameterlist];
                        DispatchEvent(parameterlist, ElementState::close, triggerField);
                    } },            
                    { "if", [this](){
                        --triggerField[ifstmt];
                        DispatchEvent(ifstmt, ElementState::close, triggerField);
                    } },            
                    { "for", [this](){
                        --triggerField[forstmt];
                        DispatchEvent(forstmt, ElementState::close, triggerField);
                    } },            
                    { "while", [this](){
                        --triggerField[whilestmt];
                        DispatchEvent(whilestmt, ElementState::close, triggerField);
                    } },
                    { "template", [this](){
                        --triggerField[templates];
                        DispatchEvent(templates, ElementState::close, triggerField);
                    } },            
                    { "argument_list", [this](){
                        --triggerField[argumentlist];
                        DispatchEvent(argumentlist, ElementState::close, triggerField);
                    } },            
                    { "call", [this](){
                        --triggerField[call];
                        DispatchEvent(call, ElementState::close, triggerField);
                    } },            
                    { "function", [this](){
                        --triggerField[function];
                        DispatchEvent(function, ElementState::close, triggerField);
                    } },
                    { "constructor", [this](){
                        --triggerField[constructor];
                        DispatchEvent(constructor, ElementState::close, triggerField);
                    } },
                    { "destructor", [this](){
                        --triggerField[destructor];
                        DispatchEvent(destructor, ElementState::close, triggerField);
                    } },
                    { "function_decl", [this](){
                        --triggerField[functiondecl];
                        DispatchEvent(functiondecl, ElementState::close, triggerField);
                    } },
                    { "constructor_decl", [this](){
                        --triggerField[constructordecl];
                        DispatchEvent(constructordecl, ElementState::close, triggerField);
                    } },
                    { "destructor_decl", [this](){
                        --triggerField[destructordecl];
                        DispatchEvent(destructordecl, ElementState::close, triggerField);
                    } },
                    { "class", [this](){
                        --triggerField[classn];
                        DispatchEvent(classn, ElementState::close, triggerField);
                    } },
                    { "struct", [this](){
                        --triggerField[classn];
                        DispatchEvent(structn, ElementState::close, triggerField);
                    } },
                    { "parameter", [this](){
                        --triggerField[parameterlist];
                        DispatchEvent(parameter, ElementState::close, triggerField);
                    } },    

                    { "member_list", [this](){
                        --triggerField[memberlist];
                        DispatchEvent(memberlist, ElementState::close, triggerField);
                    } },    
                    { "index", [this](){
                        --triggerField[index];
                        DispatchEvent(index, ElementState::close, triggerField);
                    } },    
                    { "operator", [this](){
                        --triggerField[op];
                        DispatchEvent(op, ElementState::close, triggerField);
                    } },
                    { "block", [this](){ 
                        --triggerField[block];
                        DispatchEvent(block, ElementState::close, triggerField);
                    } },
                    { "init", [this](){
                        --triggerField[init];
                        DispatchEvent(init, ElementState::close, triggerField);
                    } },    
                    { "argument", [this](){
                        --triggerField[argument];
                        DispatchEvent(argument, ElementState::close, triggerField);
                    } },    
                    { "literal", [this](){
                        --triggerField[literal];
                        DispatchEvent(literal, ElementState::close, triggerField);
                    } },    
                    { "modifier", [this](){
                        --triggerField[modifier];
                        DispatchEvent(modifier, ElementState::close, triggerField);
                    } },    
                    { "decl", [this](){
                        --triggerField[decl]; 
                        DispatchEvent(decl, ElementState::close, triggerField);
                    } },    
                    { "type", [this](){
                        --triggerField[type];
                        DispatchEvent(type, ElementState::close, triggerField);
                    } },
                    { "typedef", [this](){
                        --triggerField[typedefexpr]; 
                        DispatchEvent(typedefexpr, ElementState::close, triggerField);
                    } },    
                    { "expr", [this](){
                        --triggerField[expr];
                        DispatchEvent(expr, ElementState::close, triggerField);
                    } },    
                    { "name", [this](){
                        --triggerField[name];
                        DispatchEvent(name, ElementState::close, triggerField);
                    } },
                    { "macro", [this](){
                        --triggerField[macro];
                        DispatchEvent(macro, ElementState::close, triggerField);
                    } },
                    { "specifier", [this](){
                        --triggerField[specifier];
                        DispatchEvent(specifier, ElementState::close, triggerField);
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
            if(num_attributes >= 3){
                currentFilePath = std::string(attributes[2].value);
                currentFileLanguage = std::string(attributes[1].value);
                currentsrcMLRevision = std::string(attributes[0].value);
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
                                    const struct srcsax_attribute * attributes) {
            if(localname == "position"){
                currentLineNumber = strtoul(attributes[0].value, NULL, 0);
            }
            if(localname != ""){
                std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map.find(localname);            
                if (process != process_map.end()) {
                    process->second();
                }
            }
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
            std::unordered_map<std::string, std::function<void()>>::const_iterator process2 = process_map2.find(localname);            
            if (process2 != process_map2.end()) {
                process2->second();
            }
        }
    #pragma GCC diagnostic pop
    
    };
}
#endif