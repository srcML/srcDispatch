#include <TemplateArgumentPolicy.hpp>

#include <NamePolicy.hpp>

std::ostream & operator<<(std::ostream & out, const TemplateArgumentPolicy::TemplateArgumentData & argumentData) {

    return out;

}

TemplateArgumentPolicy::TemplateArgumentPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
      data{},
      argumentDepth(0),
      namePolicy(nullptr) {

    InitializeTemplateArgumentPolicyHandlers();

}

void TemplateArgumentPolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {

    data.templateArgument.back().first = policy->Data<NamePolicy::NameData>();

}

void * TemplateArgumentPolicy::DataInner() const {
    return new TemplateArgumentPolicy::TemplateArgumentData(data);
}

void TemplateArgumentPolicy::InitializeTemplateArgumentPolicyHandlers() {
    using namespace srcSAXEventDispatch;

    // start of policy
    openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {

        if(!argumentDepth && ctx.sawgeneric) {

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
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            data.templateArgument.push_back(std::make_pair(nullptr, TemplateArgumentPolicy::NAME));
            namePolicy = new NamePolicy{this};
            ctx.AddListenerDispatch(namePolicy);

        }

    };

    closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            if(namePolicy) {

                ctx.RemoveListenerDispatch(namePolicy);
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
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            data.templateArgument.push_back(std::make_pair(new std::string(), LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.templateArgument.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

    openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            data.templateArgument.push_back(std::make_pair(new std::string(), LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.templateArgument.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

    openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            data.templateArgument.push_back(std::make_pair(new std::string(), LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.templateArgument.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

    openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {

        // C++ has depth of 2 others 1
        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            data.templateArgument.push_back(std::make_pair(new std::string(), LITERAL));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.templateArgument.back().first)) += ctx.currentToken;
            };

        }

    };

    closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {

        if(     argumentDepth && (((argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || (argumentDepth && (argumentDepth + 1) == ctx.depth))) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

}
