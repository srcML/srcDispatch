/**
 * @file ExpressionPolicy.cpp
 *
 */

#include <ExpressionPolicySingleEvent.hpp>

std::ostream & operator<<(std::ostream & out, const ExpressionData & ex) {
    for (std::shared_ptr<ExpressionElement> item : ex.expr) {
        //out << "   Type " << item->type << " ";
        switch (item->type) {
            case ExpressionElement::NAME:    out << *(item->name);  break;
            case ExpressionElement::OP:      out << item->token;    break;
            case ExpressionElement::LITERAL: out << item->token;    break;
            case ExpressionElement::CALL:    out << *(item->call);  break;
        }
        out << " ";
    }
    return out;
}

ExpressionPolicy::~ExpressionPolicy() {
        if(namePolicy)  delete namePolicy;
        if(callPolicy)  delete callPolicy;
}

void ExpressionPolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {
    if(typeid(NamePolicy) == typeid(*policy)) {
        data.expr.push_back(std::make_shared<ExpressionElement>(ExpressionElement::NAME, policy->Data<NameData>()));
           //std::cerr << "Return Name found: " << *(data.expr.back())->name << std::endl;
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    } else if(typeid(CallPolicy) == typeid(*policy)) {
        data.expr.push_back(std::make_shared<ExpressionElement>(ExpressionElement::CALL, policy->Data<CallData>()));
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    }
    //Operators are added in CollectOtherHandlers()
}

void ExpressionPolicy::InitializeExpressionPolicyHandlers() {
    using namespace srcSAXEventDispatch;
    // start of policy
    std::function<void (srcSAXEventContext& ctx)> expressionStart = [this](srcSAXEventContext& ctx) {
        if(!exprDepth) {
            exprDepth = ctx.depth;
            data = ExpressionData{};
            data.lineNumber = ctx.currentLineNumber;
            CollectNameHandlers();
            CollectCallHandlers();
            CollectOtherHandlers();
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
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if(!namePolicy) namePolicy = new NamePolicy{this};
        ctx.dispatcher->AddListenerDispatch(namePolicy);
    };
}

void ExpressionPolicy::CollectCallHandlers() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
        if(!callPolicy) callPolicy = new CallPolicy{this};
        ctx.dispatcher->AddListenerDispatch(callPolicy);
    };
}

void ExpressionPolicy::CollectOtherHandlers() {  //Get the operators
    using namespace srcSAXEventDispatch;
    closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
        if (ctx.currentTag == "operator") {
            data.expr.push_back(std::make_shared<ExpressionElement>(ExpressionElement::OP, ctx.currentToken));
        }
        if (ctx.currentTag == "literal") {
            data.expr.push_back(std::make_shared<ExpressionElement>(ExpressionElement::LITERAL, ctx.currentToken));
        }

    };
}

