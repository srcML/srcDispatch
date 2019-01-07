/**
 * @file TemplateArgumentPolicySingleEvent.cpp
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
#include <TemplateArgumentPolicySingleEvent.hpp>

#include <NamePolicySingleEvent.hpp>

std::ostream & operator<<(std::ostream & out, const TemplateArgumentPolicy::TemplateArgumentData & argumentData) {

    for(std::size_t pos = 0; pos < argumentData.data.size(); ++pos) {

        if(pos != 0)
            out << ' ';

        const std::pair<void *, TemplateArgumentPolicy::TemplateArgumentType> & element = argumentData.data[pos];
        if(element.second == TemplateArgumentPolicy::NAME)
            out << *static_cast<NamePolicy::NameData *>(element.first);
        else if(element.second == TemplateArgumentPolicy::POINTER)
            out << '*';
        else if(element.second == TemplateArgumentPolicy::REFERENCE)
            out << '&';
        else if(element.second == TemplateArgumentPolicy::RVALUE)
            out << "&&";
        else
            out << *static_cast<std::string *>(element.first);

    }

    return out;

}

TemplateArgumentPolicy::TemplateArgumentPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
      data{},
      argumentDepth(0),
      namePolicy(nullptr) {

    InitializeTemplateArgumentPolicyHandlers();

}

TemplateArgumentPolicy::~TemplateArgumentPolicy() {

    if(namePolicy) delete namePolicy;

}

void TemplateArgumentPolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {

    data.data.back().first = policy->Data<NamePolicy::NameData>();
    ctx.dispatcher->RemoveListenerDispatch(nullptr);

}
void TemplateArgumentPolicy::NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx){}

void * TemplateArgumentPolicy::DataInner() const {
    return new TemplateArgumentPolicy::TemplateArgumentData(data);
}

void TemplateArgumentPolicy::InitializeTemplateArgumentPolicyHandlers() {
    using namespace srcSAXEventDispatch;

    // start of policy
    openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {

        if(!argumentDepth) {

            argumentDepth = ctx.depth;
            data = TemplateArgumentPolicy::TemplateArgumentData{};

            CollectNamesHandler();
            CollectOthersHandler();

        }

    };

    // end of policy
    closeEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {

        if(argumentDepth && argumentDepth == ctx.depth) {

            argumentDepth = 0;

            NotifyAll(ctx);
            InitializeTemplateArgumentPolicyHandlers();

        }
       
    };

}

void TemplateArgumentPolicy::CollectNamesHandler() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            data.data.push_back(std::make_pair(nullptr, TemplateArgumentPolicy::NAME));
            if(!namePolicy) namePolicy = new NamePolicy{this};
            ctx.dispatcher->AddListenerDispatch(namePolicy);

        }

    };

}

void TemplateArgumentPolicy::CollectOthersHandler() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            data.data.push_back(std::make_pair(new std::string(), LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

    openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            data.data.push_back(std::make_pair(new std::string(), OPERATOR));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

    openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            data.data.push_back(std::make_pair(nullptr, MODIFIER));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                if(ctx.currentToken == "*")
                    data.data.back().second = POINTER;
                else if(ctx.currentToken == "&")
                    data.data.back().second = REFERENCE;
                else if(ctx.currentToken == "&&")
                    data.data.back().second = RVALUE;

            };

        }

    };

    closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

    openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            data.data.push_back(std::make_pair(new std::string(), CALL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

}
