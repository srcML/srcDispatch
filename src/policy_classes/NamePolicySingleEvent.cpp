/**
 * @file NamePolicySingleEvent.cpp
 *
 * MODIFIED from srcSAXEventDispatcher
 *  This collects the expression in the index
 *
  */
#include <NamePolicySingleEvent.hpp>

std::string NameData::SimpleName() const {
    if (!name.empty())
        return name;
    return names.back()->SimpleName();
}


std::string NameData::ToString() const {
    std::string str = name;
    for(std::size_t pos = 0; pos < names.size(); ++pos) {
        if (pos != 0)
            str += ' ';
        str += names[pos]->ToString();
    }
    return str;
}


std::ostream & operator<<(std::ostream & out, const NameData & nameData) {
    if (!nameData.name.empty()) {
        out << nameData.name;
    }
    for (size_t pos = 0; pos < nameData.names.size(); ++pos) {
        if (pos != 0) out << "::";
        out << (*nameData.names[pos]);
    }
    if (!nameData.templateArguments.empty()) {
        out << '<';
        for(const std::shared_ptr<TemplateArgumentData> arg : nameData.templateArguments) {
            out << *arg;
        }
        out << '>';
    }
    if (nameData.indices) {
        out << '[' << *nameData.indices << ']';
    }
    return out;
}

NamePolicy::~NamePolicy() {
    if (namePolicy)             delete namePolicy;
    if (expressionPolicy)       delete expressionPolicy;
    if (templateArgumentPolicy) delete templateArgumentPolicy;
}


void NamePolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx)  {
    if (typeid(NamePolicy) == typeid(*policy)) {
        data.names.push_back(policy->Data<NameData>());
        ctx.dispatcher->RemoveListener(nullptr);
    } else if (typeid(TemplateArgumentPolicy) == typeid(*policy)) {
        data.templateArguments.push_back(policy->Data<TemplateArgumentData>());
        ctx.dispatcher->RemoveListener(nullptr);
    } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
        data.indices = policy->Data<ExpressionData>();
        ctx.dispatcher->RemoveListener(nullptr);
    }

}


void NamePolicy::InitializeNamePolicyHandlers() {
    using namespace srcSAXEventDispatch;
    // start of policy
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if (!nameDepth) {
            nameDepth = ctx.depth;
            data = NameData{};
            data.lineNumber = ctx.currentLineNumber;
            CollectTemplateArgumentsHandlers();
            CollectArrayIndicesHandlers();
        } else if ((nameDepth + 1) == ctx.depth) {
            NopCloseEvents({ParserState::tokenstring});
            if (!namePolicy) namePolicy = new NamePolicy{this};
            ctx.dispatcher->AddListenerDispatch(namePolicy);
        }
    };
    // end of policy
    closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if (nameDepth && nameDepth == ctx.depth) {
            nameDepth = 0;
            NotifyAll(ctx);
            InitializeNamePolicyHandlers();
        }
    };
    closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
        if (nameDepth && nameDepth == ctx.depth) {
            data.name += ctx.currentToken;
        }
    };
}


void NamePolicy::CollectTemplateArgumentsHandlers() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {
        if (nameDepth && (nameDepth + 1) == ctx.depth) {
            openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
                if (nameDepth && (nameDepth + 2) == ctx.depth) {
                    if (!templateArgumentPolicy) templateArgumentPolicy = new TemplateArgumentPolicy{this};
                    ctx.dispatcher->AddListenerDispatch(templateArgumentPolicy);
                }
            };
        }
    };
    closeEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {
        if (nameDepth && (nameDepth + 1) == ctx.depth) {
            NopOpenEvents({ParserState::argument});
        }
    };
}


void NamePolicy::CollectArrayIndicesHandlers() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {
        if(!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
        ctx.dispatcher->AddListenerDispatch(expressionPolicy);
    };
}

