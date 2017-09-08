/**
 * @file srcSAXEventDispatchUtilities.hpp
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
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <libxml/xmlwriter.h>
#include <srcSAXHandler.hpp>
#ifndef INCLUDED_SRCSAX_EVENT_DISPATCH_UTILITIES_HPP
#define INCLUDED_SRCSAX_EVENT_DISPATCH_UTILITIES_HPP

namespace srcSAXEventDispatch{
    class EventDispatcher;            
    enum ElementState {open, close};
    enum ParserState {decl, expr, parameter, declstmt, exprstmt, parameterlist, 
        argumentlist, argumentlisttemplate, call, templates, ctrlflow, endflow, genericargumentlist,
        name, function, functiondecl, constructor, constructordecl, destructordecl, destructor,
        argument, index, block, type, typeprev, init, op, literal, modifier, memberlist, classn, structn,
        super_list, super, publicaccess, privateaccess, protectedaccess, preproc, whilestmt, forstmt, 
        ifstmt, nonterminal, macro, classblock, functionblock, ifblock, whileblock, forblock, specifier, typedefexpr,
        userdefined, snoun, propersnoun, spronoun, sadjective, sverb, stereotype, archive, unit, returnstmt,

        // do not put anything after these
        xmlattribute, tokenstring, empty, MAXENUMVALUE = empty};
    class srcSAXEventContext {
        public:
            srcSAXEventContext() = delete;
            srcSAXEventContext(EventDispatcher * dispatcher, const std::vector<std::string> & elementStack)
                : dispatcher(dispatcher),
                  elementStack(elementStack),
                  triggerField(std::vector<unsigned short int>(MAXENUMVALUE, 0)),
                  depth(0),
                  isPrev(false),
                  isOperator(false),
                  endArchive(false),
                  currentLineNumber{0},
                  archiveBuffer{0},
                  writer{0} {}
            ~srcSAXEventContext(){
                if(writer){
                    xmlBufferFree(archiveBuffer);
                    xmlFreeTextWriter(writer);
                }
            }
            //Objects for writing to archive and obtaining archive post-parse
            xmlTextWriterPtr writer;
            xmlBufferPtr archiveBuffer;

            EventDispatcher * dispatcher;
            const std::vector<std::string> & elementStack;
            std::vector<int> genericDepth;
            unsigned int currentLineNumber;
            std::vector<unsigned short int> triggerField;
            std::string currentFilePath, currentFileName, currentFileLanguage, currentsrcMLRevision,
                        currentTag, currentToken, currentAttributeName, currentAttributeValue, currentFunctionName,
                        currentClassName;
            std::size_t depth;
            bool isPrev, isOperator, endArchive;

          /**
            * write_start_tag
            * @param localname the name of the element tag
            * @param prefix the tag prefix
            * @param URI the namespace of tag
            * @param num_namespaces number of namespaces definitions
            * @param namespaces the defined namespaces
            * @param nb_attributes the number of attributes on the tag
            * @param attributes list of attributes
            *
            * SAX handler function for start of the root element.
            * Write out a start tag.
            *
            * Overide for desired behaviour.
            */
            void write_start_tag(const char* localname, const char* prefix, const char* URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {
                xmlTextWriterStartElementNS(writer, (const xmlChar *)prefix, (const xmlChar *)localname, 0);
                bool seenpos = false;
                for(int pos = 0; pos < num_namespaces; ++pos) {
                    std::string name = "xmlns";
                    if(namespaces[pos].prefix) {
                        name += ":";
                        name += (const char *)namespaces[pos].prefix;
    
                    }
                    xmlTextWriterWriteAttribute(writer, (const xmlChar *)name.c_str(), (const xmlChar *)namespaces[pos].uri);
                }
                for(int pos = 0; pos < num_attributes; ++pos) {
                    std::string str(attributes[pos].localname);
                    if(str == "line" || str == "filename"){
                    xmlTextWriterWriteAttributeNS(writer, (const xmlChar *)attributes[pos].prefix, (const xmlChar *)attributes[pos].localname,
                        (const xmlChar *)attributes[pos].uri, (const xmlChar *)attributes[pos].value);                    
                    }
    
                }
            }
          /**
            * write_content
            * @param text_content
            *
            * Write out the provided text content, escaping everything but ".
            */
            void write_content(const std::string &text_content) {        
                if(!text_content.empty()) {        
                    /*
                        Normal output of text is for the most part
                        identical to what libxml2 provides.  However,
                        srcML does not escape " while libxml2 does escape
                        quotations.
                    */
                    int ret = 0;
                    char * text = (char *)text_content.c_str();
                    for(char * pos = text; *pos; ++pos) {       
                        if(*pos != '"') continue;       
                        
                        *pos = 0;
                        ret = xmlTextWriterWriteString(writer, (const xmlChar *)text);
                               
                        *pos = '\"';
                        xmlTextWriterWriteRaw(writer, (const xmlChar *)"\"");
                               
                        text = pos + 1;     
                    }       
                    ret = xmlTextWriterWriteString(writer, (const xmlChar *)text);
                }  
            }
            inline bool And(const std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) continue;
                    else return false;
                }
                return true;
            }
            inline bool Nand(const std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) return false;
                    else continue;
                }
                return true;
            }
            inline bool Or(const std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) return true;
                    else continue;
                }
                return false;
            }
            inline bool Nor(const std::vector<ParserState> vec) const{
                for(auto field : vec){
                    if(triggerField[field]) return false;
                    else continue;
                }
                return true;
            }
            inline bool IsEqualTo(const ParserState lhs, const ParserState rhs) const{
                return triggerField[lhs] == triggerField[rhs] ? true : false;
            }
            inline bool IsGreaterThan(const ParserState lhs, const ParserState rhs) const{
                return triggerField[lhs] > triggerField[rhs] ? true : false;
            }
            inline bool IsGreaterThanOrEqualTo(const ParserState lhs, const ParserState rhs) const{
                return triggerField[lhs] >= triggerField[rhs] ? true : false;   
            }
            inline bool IsLessThan(const ParserState lhs, const ParserState rhs) const{
                return triggerField[lhs] < triggerField[rhs] ? true : false;    
            }
            inline bool IsLessThanOrEqualTo(const ParserState lhs, const ParserState rhs) const{
                return triggerField[lhs] <= triggerField[rhs] ? true : false;   
            }
            inline bool IsOpen(const ParserState field) const{
                if(triggerField[field]) return true;
                else return false;
            }
            inline bool IsClosed(const ParserState field) const{
                if(triggerField[field]) return false;
                else return true;
            }
            inline unsigned int NumCurrentlyOpen(const ParserState field){
                return triggerField[field];
            }
    };

    class EventListener {
        typedef std::unordered_map<srcSAXEventDispatch::ParserState, std::function<void(srcSAXEventDispatch::srcSAXEventContext&)>, std::hash<int>> EventMap;
        protected:
           bool dispatched;
           EventMap openEventMap, closeEventMap;


        public:

            EventListener() : dispatched(false) {
                DefaultEventHandlers();
            }

            void SetDispatched(bool isDispatched) { dispatched = isDispatched; }

            virtual const EventMap & GetOpenEventMap() const { return openEventMap; }
            virtual const EventMap & GetCloseEventMap() const { return closeEventMap; }

            virtual void HandleEvent() { dispatched = true; }
            virtual void HandleEvent(srcSAXEventDispatch::ParserState pstate, srcSAXEventDispatch::ElementState estate, srcSAXEventDispatch::srcSAXEventContext& ctx) {

                if(dispatched) return;

                dispatched = true;

                switch(estate){

                    case srcSAXEventDispatch::ElementState::open: {
                        auto event = openEventMap.find(pstate);
                        if(event != openEventMap.end()){
                            event->second(ctx);
                        }
                        break;
                    }

                    case srcSAXEventDispatch::ElementState::close: {
                        auto event = closeEventMap.find(pstate);
                        if(event != closeEventMap.end()){
                            event->second(ctx);
                        }
                        break;
                    }

                    default:
                        throw std::runtime_error("Something went terribly, terribly wrong");

                }

            }

        protected:
            void NopOpenEvents(std::initializer_list<ParserState> states) {

                for(ParserState state : states) {

                    openEventMap[state] = [this](const srcSAXEventContext& ctx) {};

                }

            }            
            void NopCloseEvents(std::initializer_list<ParserState> states) {

                for(ParserState state : states) {

                    closeEventMap[state] = [this](const srcSAXEventContext& ctx) {};

                }

            } 

        private:

            void DefaultEventHandlers() {
                using namespace srcSAXEventDispatch;

                NopOpenEvents({
                    ParserState::declstmt,
                    ParserState::exprstmt,
                    ParserState::parameterlist,
                    ParserState::ifstmt,
                    ParserState::forstmt,
                    ParserState::whilestmt,
                    ParserState::templates,
                    ParserState::argumentlist,
                    ParserState::genericargumentlist,
                    ParserState::call,
                    ParserState::function,
                    ParserState::constructor,
                    ParserState::functiondecl,
                    ParserState::destructordecl,
                    ParserState::constructordecl,
                    ParserState::classn,
                    ParserState::structn,
                    ParserState::publicaccess,
                    ParserState::protectedaccess,
                    ParserState::privateaccess,
                    ParserState::destructor,
                    ParserState::parameter,
                    ParserState::memberlist,
                    ParserState::index,
                    ParserState::op,
                    ParserState::block,
                    ParserState::init,
                    ParserState::argument,
                    ParserState::literal,
                    ParserState::modifier,
                    ParserState::decl,
                    ParserState::type,
                    ParserState::typedefexpr,
                    ParserState::expr,
                    ParserState::name,
                    ParserState::macro,
                    ParserState::specifier,
                    ParserState::snoun,
                    ParserState::propersnoun,
                    ParserState::sadjective,
                    ParserState::spronoun,
                    ParserState::sverb,
                    ParserState::returnstmt,
                    ParserState::stereotype,
                    ParserState::archive,
                });

                NopCloseEvents({
                    ParserState::declstmt,
                    ParserState::exprstmt,
                    ParserState::parameterlist,
                    ParserState::ifstmt,
                    ParserState::forstmt,
                    ParserState::whilestmt,
                    ParserState::templates,
                    ParserState::argumentlist,
                    ParserState::genericargumentlist,
                    ParserState::call,
                    ParserState::function,
                    ParserState::constructor,
                    ParserState::destructor,
                    ParserState::functiondecl,
                    ParserState::constructordecl,
                    ParserState::destructordecl,
                    ParserState::classn,
                    ParserState::structn,
                    ParserState::publicaccess,
                    ParserState::protectedaccess,                  
                    ParserState::privateaccess,
                    ParserState::parameter,
                    ParserState::memberlist,
                    ParserState::index,
                    ParserState::op,
                    ParserState::block,
                    ParserState::init,
                    ParserState::argument,
                    ParserState::literal,
                    ParserState::modifier,
                    ParserState::decl,
                    ParserState::type,
                    ParserState::typedefexpr,
                    ParserState::expr,
                    ParserState::name,
                    ParserState::macro,
                    ParserState::tokenstring,
                    ParserState::specifier,
                    ParserState::snoun,
                    ParserState::propersnoun,
                    ParserState::sadjective,
                    ParserState::spronoun,
                    ParserState::sverb,
                    ParserState::stereotype,
                    ParserState::returnstmt,
                    ParserState::archive,
                });

        }

    };
    class EventDispatcher {
    public:
        virtual void AddListener(EventListener* l) = 0;
        virtual void AddListenerDispatch(EventListener* listener) = 0;
        virtual void AddListenerNoDispatch(EventListener* listener) = 0;
        virtual void RemoveListener(EventListener* l) = 0;
        virtual void RemoveListenerDispatch(EventListener* listener) = 0;
        virtual void RemoveListenerNoDispatch(EventListener* listener) = 0;
    protected:
        srcSAXEventContext ctx;
        std::list<EventListener*> elementListeners;

        EventDispatcher(const std::vector<std::string> & elementStack)
            : elementListeners(), ctx(this, elementStack) {}
        virtual void DispatchEvent(ParserState, ElementState) = 0;
    };
    class PolicyDispatcher;
    class PolicyListener{

        public:

            PolicyListener() {}
            virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventContext & ctx) = 0;

        };
    class PolicyDispatcher{
    public:
        PolicyDispatcher(std::initializer_list<PolicyListener *> listeners) : policyListeners(listeners){}
        virtual void AddListener(PolicyListener* listener){
            policyListeners.push_back(listener);
        }
        virtual void RemoveListener(PolicyListener* listener){
            policyListeners.erase(std::find(policyListeners.begin(), policyListeners.end(), listener));
        }

        template<typename T>
        T * Data() const {

            return static_cast<T *>(DataInner());

        }

    protected:
        std::list<PolicyListener*> policyListeners;
        virtual void * DataInner() const = 0;
        virtual void NotifyAll(const srcSAXEventContext & ctx) {
            for(std::list<PolicyListener*>::iterator listener = policyListeners.begin(); listener != policyListeners.end(); ++listener){
                (*listener)->Notify(this, ctx);
            }

        }

    };
}

#endif
