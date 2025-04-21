/**
 * @file TemplateArgumentListPolicySingleEvent.cpp
 *
 */
#include <TemplateArgumentListPolicySingleEvent.hpp>

#include <ExpressionPolicySingleEvent.hpp>

#include <sstream>

std::string TemplateArgumentListData::ToString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator<<(std::ostream& out, const TemplateArgumentListData& argumentData) {

    out << '<';
    for(std::size_t pos = 0; pos < argumentData.arguments.size(); ++pos) {
        if (pos != 0) {
            out << ", ";
        }

        out << *argumentData.arguments.at(pos);
    }
    out << '>';

    return out;
}

TemplateArgumentListPolicy::TemplateArgumentListPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      argumentListDepth(0) {
    InitializeTemplateArgumentListPolicyHandlers();
}

TemplateArgumentListPolicy::~TemplateArgumentListPolicy() {}

void TemplateArgumentListPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    if (typeid(NamePolicy) == typeid(*policy)) {
        //data.arguments.push_back(policy->Data<NameData>());
    } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
        data.arguments.push_back(policy->Data<ExpressionData>());
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListener(nullptr);
}

std::any TemplateArgumentListPolicy::DataInner() const {
    return std::make_shared<TemplateArgumentListData>(data);
}

void TemplateArgumentListPolicy::InitializeTemplateArgumentListPolicyHandlers() {
    using namespace srcDispatch;
    // start of policy
    openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {
        if (!argumentListDepth) {
            argumentListDepth = ctx.depth;
            data = TemplateArgumentListData{};
            data.lineNumber = ctx.startLineNumber;
            CollectArgumentHandler();
        }
    };

    // end of policy
    closeEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {
        if (argumentListDepth && argumentListDepth == ctx.depth) {
            argumentListDepth = 0;
            NotifyAll(ctx);
            InitializeTemplateArgumentListPolicyHandlers();
        }
    };
}

void TemplateArgumentListPolicy::CollectArgumentHandler() {
    using namespace srcDispatch;

    openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
        if(!argumentListDepth) return;
        if((argumentListDepth + 1) != ctx.depth) return;

        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if(!argumentListDepth) return;
            if((argumentListDepth + 2) != ctx.depth) return;

            if(!expressionPolicy) expressionPolicy = make_unique_policy<ExpressionPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(expressionPolicy.get());
        };

    };

    closeEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
        if(!argumentListDepth) return;
        if((argumentListDepth + 1) != ctx.depth) return;
        NopOpenEvents({ParserState::expr});
    };



}
