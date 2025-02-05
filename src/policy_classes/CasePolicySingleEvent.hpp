/**
 * @file CasePolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_CASE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CASE_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>


// Collect the expression in the return
//
class CasePolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    std::shared_ptr<ExpressionData>   data;
    std::size_t                       caseDepth;
    std::unique_ptr<ExpressionPolicy> exprPolicy;

public:
    CasePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          caseDepth(0) {
        InitializeCasePolicyHandlers();
    }

    ~CasePolicy() {}

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

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeCasePolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::switchcase] = [this](srcSAXEventContext& ctx) {
            if (!caseDepth) {
                caseDepth = ctx.depth;
                data = std::make_shared<ExpressionData>();
                CollectExpressionHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::switchcase] = [this](srcSAXEventContext& ctx) {
            if (caseDepth && caseDepth == ctx.depth) {
                caseDepth = 0;
                NotifyAll(ctx);
                InitializeCasePolicyHandlers();
            }
        };
    }

    void CollectExpressionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if (!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
        };
    }

};

#endif
