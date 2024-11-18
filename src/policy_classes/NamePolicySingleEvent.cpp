/**
 * @file NamePolicySingleEvent.cpp
 *
 * MODIFIED from srcDispatch
 *  This collects the expression in the index
 *
  */
#include <NamePolicySingleEvent.hpp>


#include <ExpressionPolicySingleEvent.hpp>
#include <cassert>

std::string NameData::SimpleName() const {
    if (!name.empty()) {
        return ToString();
    }

    assert(names.back().type() == typeid(std::shared_ptr<NameData>));
    return std::any_cast<std::shared_ptr<NameData>>(names.back())->SimpleName();
}

std::string NameData::ToString() const {

    std::string str = name;
    for(const std::any& name_element : names) {
        if(name_element.type() == typeid(std::shared_ptr<NameData>)) {
            str += std::any_cast<std::shared_ptr<NameData>>(name_element)->ToString();
        } else {
            str += std::any_cast<std::shared_ptr<OperatorData>>(name_element)->op;
        }
    }

    if (templateArgumentList) {
        str += templateArgumentList->ToString();
    }

    return str;
}


std::ostream& operator<<(std::ostream& out, const NameData& nameData) {
    out << nameData.name;

    for(const std::any& name_element : nameData.names) {
        if(name_element.type() == typeid(std::shared_ptr<NameData>)) {
            out << *std::any_cast<std::shared_ptr<NameData>>(name_element);
        } else {
            out << std::any_cast<std::shared_ptr<OperatorData>>(name_element)->op;
        }
    }
    if (nameData.templateArgumentList) {
        out << *nameData.templateArgumentList;
    }
    if (nameData.indices) {
        out << '[' << *nameData.indices << ']';
    }
    return out;
}

NamePolicy::~NamePolicy() {
    if (namePolicy)                 delete namePolicy;
    if (operatorPolicy)             delete operatorPolicy;
    if (templateArgumentListPolicy) delete templateArgumentListPolicy;
    if (expressionPolicy)           delete expressionPolicy;
}


void NamePolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx)  {

    if (typeid(NamePolicy) == typeid(*policy)) {
        data.names.push_back(policy->Data<NameData>());
    } else if (typeid(OperatorPolicy) == typeid(*policy)) {
        data.names.push_back(policy->Data<OperatorData>());
    } else if (typeid(TemplateArgumentListPolicy) == typeid(*policy)) {
        data.templateArgumentList = policy->Data<TemplateArgumentListData>();
    } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
        data.indices = policy->Data<ExpressionData>();
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListener(nullptr);
}


void NamePolicy::InitializeNamePolicyHandlers() {
    using namespace srcDispatch;
    // start of policy
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if (!nameDepth) {
            nameDepth = ctx.depth;
            data = NameData{};
            data.lineNumber = ctx.currentLineNumber;
            CollectOperatorsHandlers();
            CollectTemplateArgumentListHandlers();
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

void NamePolicy::CollectOperatorsHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
        if(!nameDepth) return;
        if((nameDepth + 1) != ctx.depth) return;

        if(!operatorPolicy) operatorPolicy = new OperatorPolicy{this};
        ctx.dispatcher->AddListenerDispatch(operatorPolicy);
    };
}


void NamePolicy::CollectTemplateArgumentListHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {
        if (!nameDepth) return;
        if((nameDepth + 1) != ctx.depth) return;

        if (!templateArgumentListPolicy) templateArgumentListPolicy = new TemplateArgumentListPolicy{this};
        ctx.dispatcher->AddListenerDispatch(templateArgumentListPolicy);
    };
}


void NamePolicy::CollectArrayIndicesHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if(!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(expressionPolicy);
        };
    };
}

