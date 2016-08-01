#include <TemplateArgumentPolicy.hpp>

#include <NamePolicy.hpp>

std::ostream & operator<<(std::ostream & out, const TemplateArgumentPolicy::TemplateArgumentData & argumentData) {

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

void TemplateArgumentPolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {

    data.types.back().first = policy->Data<NamePolicy::NameData>();

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
            CollectLiteralsHandler();
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
        if(   (argumentDepth && (argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || argumentDepth && (argumentDepth + 1) == ctx.depth) {

            data.types.push_back(std::make_pair(nullptr, TemplateArgumentPolicy::NAME));
            namePolicy = new NamePolicy{this};
            ctx.AddListenerDispatch(namePolicy);

        }

    };

    closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

        if(   (argumentDepth && (argumentDepth + 2) == ctx.depth && ctx.elementStack.back() == "expr")
            || argumentDepth && (argumentDepth + 1) == ctx.depth) {

            if(namePolicy) {

                ctx.RemoveListenerDispatch(namePolicy);
                delete namePolicy;
                namePolicy = nullptr;

            }

        }

    };

}

void TemplateArgumentPolicy::CollectLiteralsHandler() {
    using namespace srcSAXEventDispatch;
}

void TemplateArgumentPolicy::CollectOthersHandler() {
    using namespace srcSAXEventDispatch;
}
