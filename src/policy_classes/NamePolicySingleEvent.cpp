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
    if(!name.empty()) {
        return ToString();
    }

    assert(names.back().type() == typeid(std::shared_ptr<NameData>));
    return std::any_cast<std::shared_ptr<NameData>>(names.back())->SimpleName();
}

std::string NameData::ToString() const {

    std::ostringstream out;
    out << *this;

    return out.str();
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
    if(nameData.templateArgumentList) {
        out << *nameData.templateArgumentList;
    }

    if(nameData.indices) {
        out << '[';

        bool printComma = false;
        for (const std::shared_ptr<ExpressionData>& index : *nameData.indices) {

            if(printComma) {
                out << ", ";
            }

            out << *index;
            printComma = true;
        }

        out << ']';
    }

    return out;
}

NamePolicy::~NamePolicy() {}


void NamePolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx)  {

    if(typeid(NamePolicy) == typeid(*policy)) {
        data.names.push_back(policy->Data<NameData>());
    } else if(typeid(OperatorPolicy) == typeid(*policy)) {
        data.names.push_back(policy->Data<OperatorData>());
    } else if(typeid(TemplateArgumentListPolicy) == typeid(*policy)) {
        data.templateArgumentList = policy->Data<TemplateArgumentListData>();
    } else if(typeid(ExpressionPolicy) == typeid(*policy)) {
        data.indices->push_back(policy->Data<ExpressionData>());
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListener(nullptr);
}


void NamePolicy::InitializeNamePolicyHandlers() {
    using namespace srcDispatch;

    // start of policy
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if(!nameDepth) {
            nameDepth = ctx.depth;
            data = NameData{};
            data.lineNumber = ctx.startLineNumber;
            CollectOperatorsHandlers();
            CollectTemplateArgumentListHandlers();
            CollectArrayIndicesHandlers();
        } else if((nameDepth + 1) == ctx.depth) {
            NopCloseEvents({ParserState::tokenstring});
            if(!namePolicy) namePolicy = make_unique_policy<NamePolicy>({this});
            ctx.dispatcher->AddListenerDispatch(namePolicy.get());
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

void NamePolicy::CollectOperatorsHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
        if(!nameDepth) return;
        if((nameDepth + 1) != ctx.depth) return;

        if(!operatorPolicy) operatorPolicy = make_unique_policy<OperatorPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(operatorPolicy.get());
    };
}


void NamePolicy::CollectTemplateArgumentListHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext& ctx) {
        if(!nameDepth) return;
        if((nameDepth + 1) != ctx.depth) return;

        if(!templateArgumentListPolicy) templateArgumentListPolicy = make_unique_policy<TemplateArgumentListPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(templateArgumentListPolicy.get());
    };
}


void NamePolicy::CollectArrayIndicesHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {
        data.indices = std::vector<std::shared_ptr<ExpressionData>>();
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if(!expressionPolicy) expressionPolicy = make_unique_policy<ExpressionPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(expressionPolicy.get());
        };
    };
}

