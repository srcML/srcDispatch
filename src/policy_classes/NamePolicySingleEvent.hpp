/**
 * @file NamePolicySingleEvent.hpp
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
#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <TemplateArgumentPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

#ifndef INCLUDED_NAME_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_NAME_POLICY_SINGLE_EVENT_HPP

class NamePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct NameData {

        std::string name;
        std::vector<NameData *> names;
        std::vector<TemplateArgumentPolicy::TemplateArgumentData *> templateArguments;
        std::vector<std::string> arrayIndices;

        std::string SimpleName() const {

            if(!name.empty())
                return name;

            return names.back()->SimpleName();

        }

        std::string ToString() const {

            std::string str = name;

            for(std::size_t pos = 0; pos < names.size(); ++pos) {

                if(pos != 0)
                    str += ' ';
                str += names[pos]->ToString();

            }

            return str;

        }

        friend std::ostream & operator<<(std::ostream & out, const NameData & nameData) {

            if(!nameData.name.empty()) {
                out << nameData.name;
            }

            for(size_t pos = 0; pos < nameData.names.size(); ++pos) {

                if(pos != 0) out << "::";
                out << (*nameData.names[pos]);

            }

            if(!nameData.templateArguments.empty()) {
                out << '<';
                for(const TemplateArgumentPolicy::TemplateArgumentData * arg : nameData.templateArguments) {
                    out << *arg;
                }
                out << '>';
            }

            for(const std::string & index : nameData.arrayIndices) {
                out << '[' << index << ']';
            }

            return out;


        }

    };

private:

    NameData data;
    std::size_t nameDepth;

    NamePolicy * namePolicy;
    TemplateArgumentPolicy * templateArgumentPolicy;

public:


    NamePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          nameDepth(0),
          namePolicy(nullptr),
          templateArgumentPolicy(nullptr) { 
    
        InitializeNamePolicyHandlers();

    }

    ~NamePolicy() {

        if(namePolicy) delete namePolicy;
        if(templateArgumentPolicy) delete templateArgumentPolicy;

    }

protected:
    void * DataInner() const override {

        return new NameData(data);

    }
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(NamePolicy) == typeid(*policy)) {

            data.names.push_back(policy->Data<NameData>());
            ctx.dispatcher->RemoveListener(nullptr);

        } else if(typeid(TemplateArgumentPolicy) == typeid(*policy)) {

            data.templateArguments.push_back(policy->Data<TemplateArgumentPolicy::TemplateArgumentData>());
            ctx.dispatcher->RemoveListener(nullptr);

        }

    }
    void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

private:

    void InitializeNamePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(!nameDepth) {

                nameDepth = ctx.depth;
                data = NameData{};

                CollectTemplateArgumentsHandlers();
                CollectArrayIndicesHandlers();

            } else if((nameDepth + 1) == ctx.depth) {

                NopCloseEvents({ParserState::tokenstring});
                if(!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy); 

            }

        };

        // end of policy
        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                nameDepth = 0;
 
                NotifyAll(ctx);
                InitializeNamePolicyHandlers();

            }
           
        };

        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                data.name += ctx.currentToken;

            }

        };

    }

    void CollectTemplateArgumentsHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {

                    if(nameDepth && (nameDepth + 2) == ctx.depth) {

                        if(!templateArgumentPolicy) templateArgumentPolicy = new TemplateArgumentPolicy{this};
                        ctx.dispatcher->AddListenerDispatch(templateArgumentPolicy);

                    }

                };

            }

        };

        closeEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::argument});

            }

        };

    }

    void CollectArrayIndicesHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                data.arrayIndices.push_back(std::string());

            }

        };

        closeEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::expr});
                NopCloseEvents({ParserState::expr});

            }

        };

        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {

            size_t num_elements = ctx.elementStack.size();
            if(nameDepth && (nameDepth + 2) == ctx.depth && num_elements > 1 && ctx.elementStack[num_elements - 2] == "index") {

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) { data.arrayIndices.back() += ctx.currentToken; };

            }

        };

        closeEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {

            size_t num_elements = ctx.elementStack.size();
            if(nameDepth && (nameDepth + 2) == ctx.depth && num_elements > 0 && ctx.elementStack.back() == "index") {

                NopCloseEvents({ParserState::tokenstring});

            }

        };

    }

};

#endif
