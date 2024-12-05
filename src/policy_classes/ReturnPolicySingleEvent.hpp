/**
 * @file ReturnPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_RETURN_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_RETURN_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>


// Collect the expression in the return
//
class ReturnPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    std::shared_ptr<ExpressionData>   data;
    std::size_t                       returnDepth;
    std::unique_ptr<ExpressionPolicy> exprPolicy;

public:
    ReturnPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          returnDepth(0) {
        InitializeReturnPolicyHandlers();
    }

    ~ReturnPolicy() {}

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
    void InitializeReturnPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
            if (!returnDepth) {
                returnDepth = ctx.depth;
                data = std::make_shared<ExpressionData>();
                CollectExpressionHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
            if (returnDepth && returnDepth == ctx.depth) {
                returnDepth = 0;
                NotifyAll(ctx);
                InitializeReturnPolicyHandlers();
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
