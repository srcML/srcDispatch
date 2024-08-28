/**
 * @file TemplateArgumentPolicySingleEvent.cpp
 *
 */
#include <TemplateArgumentPolicySingleEvent.hpp>

std::ostream & operator<<(std::ostream & out, const TemplateArgumentData & argumentData) {
    for(std::size_t pos = 0; pos < argumentData.data.size(); ++pos) {
        if (pos != 0)
            out << ' ';
        const std::pair<void *, TemplateArgumentData::TemplateArgumentType> & element = argumentData.data[pos];
        if (element.second == TemplateArgumentData::NAME)
            out << *static_cast<NameData *>(element.first);
        else if (element.second == TemplateArgumentData::POINTER)
            out << '*';
        else if (element.second == TemplateArgumentData::REFERENCE)
            out << '&';
        else if (element.second == TemplateArgumentData::RVALUE)
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
    if (namePolicy) delete namePolicy;
}

void TemplateArgumentPolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {
    data.data.back().first = policy->Data<NameData>();
    ctx.dispatcher->RemoveListenerDispatch(nullptr);
}

void TemplateArgumentPolicy::NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx){}

void * TemplateArgumentPolicy::DataInner() const {
    return new TemplateArgumentData(data);
}

void TemplateArgumentPolicy::InitializeTemplateArgumentPolicyHandlers() {
    using namespace srcSAXEventDispatch;
    // start of policy
    openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
        if (!argumentDepth) {
            argumentDepth = ctx.depth;
            data = TemplateArgumentData{};
            CollectNamesHandler();
            CollectOthersHandler();
        }
    };

    // end of policy
    closeEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
        if (argumentDepth && argumentDepth == ctx.depth) {
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
        if (argumentDepth &&
           (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            data.data.push_back(std::make_pair(nullptr, TemplateArgumentData::NAME));
            if (!namePolicy) namePolicy = new NamePolicy{this};
            ctx.dispatcher->AddListenerDispatch(namePolicy);
        }
    };
}

void TemplateArgumentPolicy::CollectOthersHandler() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {
        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if (argumentDepth &&
           (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            data.data.push_back(std::make_pair(new std::string(), TemplateArgumentData::LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
            };
        }
    };

    closeEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {
        if (argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            NopCloseEvents({ParserState::tokenstring});
        }
    };

    openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if (argumentDepth &&
           (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
           || (argumentDepth + 1) == ctx.depth)) {
            data.data.push_back(std::make_pair(new std::string(), TemplateArgumentData::OPERATOR));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
            };
        }
    };

    closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
        if (argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            NopCloseEvents({ParserState::tokenstring});
        }
    };

    openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if (argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            data.data.push_back(std::make_pair(nullptr, TemplateArgumentData::MODIFIER));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                if (ctx.currentToken == "*")
                    data.data.back().second = TemplateArgumentData::POINTER;
                else if (ctx.currentToken == "&")
                    data.data.back().second = TemplateArgumentData::REFERENCE;
                else if (ctx.currentToken == "&&")
                    data.data.back().second = TemplateArgumentData::RVALUE;
            };
        }
    };

    closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
        if (argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            NopCloseEvents({ParserState::tokenstring});
        }
    };

    openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
        // C++ has depth of 2 others 1
        std::size_t elementStackSize = ctx.elementStack.size();
        if (argumentDepth && (((argumentDepth + 2) == ctx.depth && elementStackSize > 1 && ctx.elementStack[elementStackSize - 2] == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            data.data.push_back(std::make_pair(new std::string(), TemplateArgumentData::CALL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
            };
        }
    };

    closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
        if (argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {
            NopCloseEvents({ParserState::tokenstring});
        }
    };
}
