#include <TemplateArgumentPolicy.hpp>

#include <NamePolicy.hpp>

std::ostream & operator<<(std::ostream & out, const TemplateArgumentPolicy::TemplateArgumentData & argumentData) {

    for(std::size_t pos = 0; pos < argumentData.data.size(); ++pos) {

        if(pos != 0)
            out << ' ';

        const std::pair<void *, TemplateArgumentPolicy::TemplateArgumentType> & element = argumentData.data[pos];
        if(element.second == TemplateArgumentPolicy::NAME)
            out << *static_cast<NamePolicy::NameData *>(element.first);
        else
            out << *static_cast<std::string *>(element.first);

    }

    return out;

}

TemplateArgumentPolicy::TemplateArgumentPolicy(SingleEventPolicyDispatcher & policy_handler, std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
      policy_handler(policy_handler),
      data{},
      argumentDepth(0),
      namePolicy(nullptr) {

    InitializeTemplateArgumentPolicyHandlers();

}

void TemplateArgumentPolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {

    data.data.back().first = policy->Data<NamePolicy::NameData>();
    policy_handler.PopListenerDispatch();

}

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
            namePolicy = new NamePolicy(policy_handler, {this});
            policy_handler.PushListenerDispatch(namePolicy);

        }

    };

    closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth + 1) == ctx.depth)) {

            if(namePolicy) {

                delete namePolicy;
                namePolicy = nullptr;

            }

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

            data.data.push_back(std::make_pair(new std::string(), LITERAL));
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

            data.data.push_back(std::make_pair(new std::string(), LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.data.back().first)) += ctx.currentToken;
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

            data.data.push_back(std::make_pair(new std::string(), LITERAL));
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
