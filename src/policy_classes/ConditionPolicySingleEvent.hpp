/**
 * @file ConditionPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_CONDITION_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CONDITION_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>


// Collect the expression in the return
//
class ConditionPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    std::shared_ptr<ExpressionData> data;
    std::size_t                     conditionDepth;
    ExpressionPolicy*               exprPolicy;

public:
    ConditionPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          conditionDepth(0),
          exprPolicy(nullptr) {
        InitializeConditionPolicyHandlers();
    }

    ~ConditionPolicy() {
        if (exprPolicy) delete exprPolicy;
    }

protected:
    std::any DataInner() const override { return data; }

    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override {
        if (typeid(ExpressionPolicy) == typeid(*policy)) {
            data = policy->Data<ExpressionData>();
            ctx.dispatcher->RemoveListener(nullptr);
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
    }

    void NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

private:
    void InitializeConditionPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if (!conditionDepth) {
                conditionDepth = ctx.depth;
                data = std::make_shared<ExpressionData>();
                CollectExpressionHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if (conditionDepth && conditionDepth == ctx.depth) {
                conditionDepth = 0;
                NotifyAll(ctx);
                InitializeConditionPolicyHandlers();
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
