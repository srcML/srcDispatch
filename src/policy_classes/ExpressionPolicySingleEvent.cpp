/**
 * @file ExpressionPolicy.cpp
 *
 */

#include <ExpressionPolicySingleEvent.hpp>

std::ostream& operator<<(std::ostream& out, const ExpressionData& ex) {
    for (std::any item : ex.expr) {
        if(item.type() == typeid(std::shared_ptr<NameData>)) {
            out << *std::any_cast<std::shared_ptr<NameData>>(item);
        } else if(item.type() == typeid(std::shared_ptr<OperatorData>)) {
            out << *std::any_cast<std::shared_ptr<OperatorData>>(item);
        } else if(item.type() == typeid(std::shared_ptr<LiteralData>)) {
            out << *std::any_cast<std::shared_ptr<LiteralData>>(item);
        } else if(item.type() == typeid(std::shared_ptr<CallData>)) {
            out << *std::any_cast<std::shared_ptr<CallData>>(item);
        }
        out << " ";
    }
    return out;
}

ExpressionPolicy::~ExpressionPolicy() {
        if(namePolicy)     delete namePolicy;
        if(operatorPolicy) delete operatorPolicy;
        if(literalPolicy)  delete literalPolicy;
        if(callPolicy)     delete callPolicy;
}

void ExpressionPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    if(typeid(NamePolicy) == typeid(*policy)) {
        data.expr.push_back(policy->Data<NameData>());
    } else if(typeid(OperatorPolicy) == typeid(*policy)) {
        data.expr.push_back(policy->Data<OperatorData>());
    }  else if(typeid(LiteralPolicy) == typeid(*policy)) {
        data.expr.push_back(policy->Data<LiteralData>());
    }  else if(typeid(CallPolicy) == typeid(*policy)) {
        data.expr.push_back(policy->Data<CallData>());
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    //Operators are added in CollectOtherHandlers()
    ctx.dispatcher->RemoveListenerDispatch(nullptr);
}

void ExpressionPolicy::InitializeExpressionPolicyHandlers() {
    using namespace srcDispatch;
    // start of policy
    std::function<void (srcSAXEventContext& ctx)> expressionStart = [this](srcSAXEventContext& ctx) {
        if(!exprDepth) {
            exprDepth = ctx.depth;
            data = ExpressionData{};
            data.lineNumber = ctx.currentLineNumber;
            CollectNameHandlers();
            CollectCallHandlers();
            CollectOperatorHandlers();
            CollectLiteralHandlers();
        }
    };

    // end of policy
    std::function<void (srcSAXEventContext& ctx)> expressionEnd = [this](srcSAXEventContext& ctx) {
        if(exprDepth && exprDepth == ctx.depth) {
            exprDepth = 0;
            NotifyAll(ctx);
            InitializeExpressionPolicyHandlers();
        }
    };

    openEventMap[ParserState::expr] = expressionStart;
    closeEventMap[ParserState::expr] = expressionEnd;
}


void ExpressionPolicy::CollectNameHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if(!namePolicy) namePolicy = new NamePolicy{this};
        ctx.dispatcher->AddListenerDispatch(namePolicy);
    };
}

void ExpressionPolicy::CollectCallHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
        if(!callPolicy) callPolicy = new CallPolicy{this};
        ctx.dispatcher->AddListenerDispatch(callPolicy);
    };
}

void ExpressionPolicy::CollectOperatorHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {
        if(!operatorPolicy) operatorPolicy = new OperatorPolicy{this};
        ctx.dispatcher->AddListenerDispatch(operatorPolicy);
    };
}

void ExpressionPolicy::CollectLiteralHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {
        if(!literalPolicy) literalPolicy = new LiteralPolicy{this};
        ctx.dispatcher->AddListenerDispatch(literalPolicy);
    };
}
