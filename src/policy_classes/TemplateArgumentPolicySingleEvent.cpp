/**
 * @file TemplateArgumentPolicySingleEvent.cpp
 *
 */
#include <TemplateArgumentPolicySingleEvent.hpp>

#include <ExpressionPolicySingleEvent.hpp>

#include <sstream>

std::string TemplateArgumentData::ToString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator<<(std::ostream& out, const TemplateArgumentData& argumentData) {

    for(std::size_t pos = 0; pos < argumentData.arguments.size(); ++pos) {
        if (pos != 0) {
            out << ", ";
        }

        out << *argumentData.arguments.at(pos);
    }

    return out;
}

TemplateArgumentPolicy::TemplateArgumentPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      argumentDepth(0),
      namePolicy(nullptr) {
    InitializeTemplateArgumentPolicyHandlers();
}

TemplateArgumentPolicy::~TemplateArgumentPolicy() {
    if (namePolicy)       delete namePolicy;
    if (expressionPolicy) delete expressionPolicy;
}

void TemplateArgumentPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    if (typeid(NamePolicy) == typeid(*policy)) {
        //data.arguments.push_back(policy->Data<NameData>());
    } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
        data.arguments.push_back(policy->Data<ExpressionData>());
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListener(nullptr);
}

void TemplateArgumentPolicy::NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]){}

std::any TemplateArgumentPolicy::DataInner() const {
    return std::make_shared<TemplateArgumentData>(data);
}

void TemplateArgumentPolicy::InitializeTemplateArgumentPolicyHandlers() {
    using namespace srcDispatch;
    // start of policy
    openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
        if (!argumentDepth) {
            argumentDepth = ctx.depth;
            data = TemplateArgumentData{};
            data.lineNumber = ctx.currentLineNumber;
            CollectExpressionHandler();
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

void TemplateArgumentPolicy::CollectExpressionHandler() {
    using namespace srcDispatch;
    openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
        if(!argumentDepth) return;
        if((argumentDepth + 1) != ctx.depth) return;

        if(!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
        ctx.dispatcher->AddListenerDispatch(expressionPolicy);
    };
}
