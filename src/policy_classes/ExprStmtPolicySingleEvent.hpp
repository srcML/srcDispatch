/**
 * @file ExprStmtPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_EXPR_STMT_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_EXPR_STMT_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>


class ExprStmtPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    std::shared_ptr<ExpressionData> data;
    std::size_t                     exprStmtDepth;
    ExpressionPolicy*               exprPolicy;

public:
    ExprStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          exprStmtDepth(0),
          exprPolicy(nullptr) {
        InitializeExprStmtPolicyHandlers();
    }

    ~ExprStmtPolicy() {
        if (exprPolicy) delete exprPolicy;
    }

protected:
    std::any DataInner() const override { return data; }

    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override {
        if (typeid(ExpressionPolicy) == typeid(*policy)) {
            data = policy->Data<ExpressionData>();
            ctx.dispatcher->RemoveListener(nullptr);
        }
    }

    void NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

private:
    void InitializeExprStmtPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
            if (!exprStmtDepth) {
                exprStmtDepth = ctx.depth;
                data = std::make_shared<ExpressionData>();
                CollectExpressionHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
            if (exprStmtDepth && exprStmtDepth == ctx.depth) {
                exprStmtDepth = 0;
                NotifyAll(ctx);
                InitializeExprStmtPolicyHandlers();
            }
        };
    }

    void CollectExpressionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if (!exprPolicy) exprPolicy = new ExpressionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(exprPolicy);
        };
    }

};

#endif
