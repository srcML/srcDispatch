/**
 * @file CallPolicySingleEvent.cpp
 *
 */
#include <CallPolicySingleEvent.hpp>

std::ostream& operator<<(std::ostream& out, const CallData& call) {
    out << *(call.name) << "(";
    bool printComma=false;
    for (std::shared_ptr<ExpressionData> arg : call.arguments) {
        if (printComma) out << ", ";
        out << *arg;
        printComma = true;
    }
    out << ")";
    return out;
}

CallPolicy::~CallPolicy() {
    if (namePolicy)       delete namePolicy;
    if (expressionPolicy) delete expressionPolicy;
}

std::any CallPolicy::DataInner() const { return std::make_shared<CallData>(data); }

void CallPolicy::Notify(const srcSAXEventDispatch::PolicyDispatcher* policy, const srcSAXEventDispatch::srcSAXEventContext& ctx) {
    using namespace srcSAXEventDispatch;
    if (typeid(NamePolicy) == typeid(*policy)) {
        data.name = policy->Data<NameData>();
        ctx.dispatcher->RemoveListener(nullptr);
    }
    if (typeid(ExpressionPolicy) == typeid(*policy)) {
        data.arguments.push_back(policy->Data<ExpressionData>());
        ctx.dispatcher->RemoveListener(nullptr);
    }
}

void CallPolicy::InitializeCallPolicyHandlers() {
    using namespace srcSAXEventDispatch;
    // start of policy
    openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
        if (!callDepth) {
            callDepth = ctx.depth;
            data = CallData{};
            data.lineNumber = ctx.currentLineNumber;
            CollectNameHandlers();
            CollectCallArgumentHandlers();
        }
    };

    // end of policy
    closeEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
        if (callDepth && callDepth == ctx.depth) {
            callDepth = 0;
            NotifyAll(ctx);
            InitializeCallPolicyHandlers();
        }
    };
}


void CallPolicy::CollectNameHandlers() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if(!namePolicy) namePolicy = new NamePolicy{this};
        ctx.dispatcher->AddListenerDispatch(namePolicy);
    };
}


void CallPolicy::CollectCallArgumentHandlers() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {
        if(!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
        ctx.dispatcher->AddListenerDispatch(expressionPolicy);
    };
}

