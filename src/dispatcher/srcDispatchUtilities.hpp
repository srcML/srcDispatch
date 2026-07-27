/**
 * @file srcDispatchUtilities.hpp
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
#include <any>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <Diff.hpp>
#include <DeltaElement.hpp>
#include <Position.hpp>

#include <libxml/xmlwriter.h>
#include <srcSAXHandler.hpp>

#ifndef INCLUDED_SRCSAX_EVENT_DISPATCH_UTILITIES_HPP
#define INCLUDED_SRCSAX_EVENT_DISPATCH_UTILITIES_HPP

namespace srcDispatch {

    class EventDispatcher;            
    enum ElementState {open, close};
    enum ParserState {decl, expr, parameter, declstmt, exprstmt, parameterlist, elseif, elsestmt, argumentlist, argumentlisttemplate, 
        call, templates, ctrlflow, endflow, genericargumentlist, name, function, functiondecl, constructor, constructordecl,
        destructordecl, destructor, argument, index, block, type, typeprev, init, op, literal, 
        modifier, memberlist, classn, structn, namespacen, super_list, super, publicaccess, privateaccess, protectedaccess,
        preproc, whilestmt, forstmt, ifstmt, nonterminal, macro, switchstmt, switchcase, specifier, throws, 
        typedefdecl, userdefined, comment, annotation, condition, gotostmt, breakstmt, continuestmt, label, throwstmt,
        trystmt, catchstmt, dostmt, incr, decr, control, ifgroup, range, returnstmt,

        foreach,

        // preprocessor
        cppinclude, cppdirective, cppfile,

        // NLP states
        snoun, propersnoun, spronoun, sadjective, sverb,

        // stereotype state
        stereotype, 

        archive, unit,

        // do not put anything after these
        xmlattribute, tokenstring, empty, MAXENUMVALUE = empty};

    class srcSAXEventContext {
        public:
            srcSAXEventContext() = delete;
            srcSAXEventContext(EventDispatcher * dispatcher, const std::vector<std::string> & elementStack)
                : writer{0},
                  archiveBuffer{0},
                  dispatcher(dispatcher),
                  elementStack(elementStack),
                  diffStack{COMMON},
                  depth(0),
                  genericDepth(),
                  startPosition(),
                  endPosition(),
                  triggerField(std::vector<unsigned short int>(MAXENUMVALUE, 0)),
                  isArchive(false),
                  isPrev(false),
                  isOperator(false),
                  endArchive(false) {}

            ~srcSAXEventContext() {
                if(writer) {
                    xmlBufferFree(archiveBuffer);
                    xmlFreeTextWriter(writer);
                }
            }
            //Objects for writing to archive and obtaining archive post-parse
            xmlTextWriterPtr writer;
            xmlBufferPtr archiveBuffer;

            EventDispatcher * dispatcher;

            const std::vector<std::string>& elementStack;
            std::vector<Diff>               diffStack;

            std::size_t depth;
            std::vector<unsigned int> genericDepth;

            DeltaElement<Position> startPosition;
            DeltaElement<Position> endPosition;

            std::vector<unsigned short int> triggerField;
            std::string currentFilePath, currentFileName, currentFileLanguage, currentsrcMLRevision,
                        currentTag, currentToken, currentAttributeName, currentAttributeValue,
                        currentClassName, currentFileChecksum;

            std::vector<std::string>  currentNamespaces;

            std::map<std::string, std::string> attributes;
            bool isArchive, isPrev, isOperator, isPseudo, endArchive;

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
            void write_start_tag(const char* localname, const char* prefix, const char* URI [[maybe_unused]],
                                int num_namespaces [[maybe_unused]], const struct srcsax_namespace* namespaces [[maybe_unused]], int num_attributes,
                                const struct srcsax_attribute* attributes) {
                xmlTextWriterStartElementNS(writer, (const xmlChar*)prefix, (const xmlChar*)localname, 0);
                for(int pos = 0; pos < num_attributes; ++pos) {
                    std::string str(attributes[pos].localname);
                    xmlTextWriterWriteAttributeNS(writer, (const xmlChar*)attributes[pos].prefix, (const xmlChar*)attributes[pos].localname,
                        (const xmlChar*)attributes[pos].uri, (const xmlChar*)attributes[pos].value);
                }
            }
          /**
            * write_content
            * @param text_content
            *
            * Write out the provided text content, escaping everything but ".
            */
            void write_content(const std::string& text_content) {        
                if(!text_content.empty()) {        
                    /*
                        Normal output of text is for the most part
                        identical to what libxml2 provides.  However,
                        srcML does not escape " while libxml2 does escape
                        quotations.
                    */
                    char * text = (char *)text_content.c_str();
                    for(char * pos = text; *pos; ++pos) {       
                        if(*pos != '"') continue;       
                        
                        *pos = 0;
                        xmlTextWriterWriteString(writer, (const xmlChar *)text);
                               
                        *pos = '\"';
                        xmlTextWriterWriteRaw(writer, (const xmlChar *)"\"");
                               
                        text = pos + 1;     
                    }       
                    xmlTextWriterWriteString(writer, (const xmlChar *)text);
                }  
            }
            inline bool And(const std::vector<ParserState> vec) const {
                for(auto field : vec) {
                    if(triggerField[field]) continue;
                    else return false;
                }
                return true;
            }
            inline bool Nand(const std::vector<ParserState> vec) const {
                for(auto field : vec) {
                    if(triggerField[field]) return false;
                    else continue;
                }
                return true;
            }
            inline bool Or(const std::vector<ParserState> vec) const {
                for(auto field : vec) {
                    if(triggerField[field]) return true;
                    else continue;
                }
                return false;
            }
            inline bool Nor(const std::vector<ParserState> vec) const {
                for(auto field : vec) {
                    if(triggerField[field]) return false;
                    else continue;
                }
                return true;
            }
            inline bool IsEqualTo(const ParserState lhs, const ParserState rhs) const {
                return triggerField[lhs] == triggerField[rhs] ? true : false;
            }
            inline bool IsGreaterThan(const ParserState lhs, const ParserState rhs) const {
                return triggerField[lhs] > triggerField[rhs] ? true : false;
            }
            inline bool IsGreaterThanOrEqualTo(const ParserState lhs, const ParserState rhs) const {
                return triggerField[lhs] >= triggerField[rhs] ? true : false;   
            }
            inline bool IsLessThan(const ParserState lhs, const ParserState rhs) const {
                return triggerField[lhs] < triggerField[rhs] ? true : false;    
            }
            inline bool IsLessThanOrEqualTo(const ParserState lhs, const ParserState rhs) const {
                return triggerField[lhs] <= triggerField[rhs] ? true : false;   
            }
            inline bool IsOpen(const ParserState field) const {
                if(triggerField[field]) return true;
                else return false;
            }
            inline bool IsClosed(const ParserState field) const {
                if(triggerField[field]) return false;
                else return true;
            }
            inline unsigned int NumCurrentlyOpen(const ParserState field) {
                return triggerField[field];
            }
    };

    class EventError : public std::runtime_error { 
        public: EventError(const std::string& msg) : std::runtime_error(msg) {}
    };

    class EventListener {
        typedef std::unordered_map<srcDispatch::ParserState, std::function<void(srcDispatch::srcSAXEventContext&)>, std::hash<int>> EventMap;
        protected:
           std::size_t depth;

           bool dispatched;
           EventMap openEventMap, closeEventMap;

        public:

            EventListener() : depth(0), dispatched(false) {
                DefaultEventHandlers();
            }

            virtual ~EventListener() {}

            void SetDispatched(bool isDispatched) { dispatched = isDispatched; }

            virtual const EventMap& GetOpenEventMap()  const { return openEventMap;  }
            virtual const EventMap& GetCloseEventMap() const { return closeEventMap; }

            virtual void HandleEvent() { dispatched = true; }
            virtual void HandleEvent(srcDispatch::ParserState pstate, srcDispatch::ElementState estate, srcDispatch::srcSAXEventContext& ctx) {

                if(dispatched) return;

                dispatched = true;

                switch(estate) {

                    case srcDispatch::ElementState::open: {
                        auto event = openEventMap.find(pstate);
                        if(event != openEventMap.end()) {
                            event->second(ctx);
                        }
                        break;
                    }

                    case srcDispatch::ElementState::close: {
                        auto event = closeEventMap.find(pstate);
                        if(event != closeEventMap.end()) {
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
                    openEventMap[state] = [](const srcSAXEventContext& ctx [[maybe_unused]]) {};
                }

            }            
            void NopCloseEvents(std::initializer_list<ParserState> states) {

                for(ParserState state : states) {
                    closeEventMap[state] = [](const srcSAXEventContext& ctx [[maybe_unused]]) {};
                }

            } 

        private:

            void DefaultEventHandlers() {
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
        xmlBufferPtr GetXmlBuffer() {return ctx.archiveBuffer;}
    protected:
        srcSAXEventContext ctx;
        std::list<EventListener*> elementListeners;

        ParserState currentPState;
        ElementState currentEState;

        EventDispatcher(const std::vector<std::string>& elementStack)
            : ctx(this, elementStack), elementListeners() {}
        virtual ~EventDispatcher() {}
        virtual void DispatchEvent(ParserState, ElementState) = 0;

public:
        const srcSAXEventContext& GetContext() const {
            return ctx;
        }

        srcSAXEventContext& GetContext() {
            return ctx;
        }

        ParserState CurrentPState() const {
            return currentPState;
        }

        ElementState CurrentEState() const {
            return currentEState;
        }

    };

    class PolicyError : public std::runtime_error { 
        public: PolicyError(const std::string& msg) : std::runtime_error(msg) {}
    };
    class PolicyDispatcher;
    class PolicyListener {

        public:

            PolicyListener() {}
            virtual ~PolicyListener() {}
            virtual void Notify(const PolicyDispatcher* policy, const srcSAXEventContext& ctx) = 0;
            virtual void NotifyWrite(const PolicyDispatcher* policy, srcSAXEventContext& ctx) = 0;
        };
    class PolicyDispatcher {
    public:
        PolicyDispatcher(std::initializer_list<PolicyListener*> listeners) : policyListeners(listeners) {}
        virtual ~PolicyDispatcher() {}
        virtual void AddListener(PolicyListener* listener) {
            policyListeners.push_back(listener);
        }
        virtual void RemoveListener(PolicyListener* listener) {
            policyListeners.erase(std::find(policyListeners.begin(), policyListeners.end(), listener));
        }

        template<typename T>
        std::shared_ptr<T> Data() const {
            return std::any_cast<std::shared_ptr<T>>(DataInner());
        }

    protected:
        std::list<PolicyListener*> policyListeners;
        virtual std::any DataInner() const = 0;
        //TODO: These may not need to be synchronous or even called in the same method (i.e., notifyall)
        virtual void NotifyAll(/*const*/ srcSAXEventContext& ctx) {
            for(std::list<PolicyListener*>::iterator listener = policyListeners.begin(); listener != policyListeners.end(); ++listener) {
                (*listener)->Notify(this, ctx);
            }
            for(std::list<PolicyListener*>::iterator listener = policyListeners.begin(); listener != policyListeners.end(); ++listener) {
                (*listener)->NotifyWrite(this, ctx);
            }

        }

    };

    template<class Policy>
    constexpr std::unique_ptr<Policy> make_unique_policy(const std::initializer_list<PolicyListener*>& args) {
        return std::make_unique<Policy>(args);
    }

}

#endif
