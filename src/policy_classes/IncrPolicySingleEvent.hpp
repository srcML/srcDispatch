/**
 * @file IncrPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_INCR_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_INCR_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ExpressionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct IncrData {

    unsigned int startLineNumber;
    std::shared_ptr<ExpressionData> expr;

    friend std::ostream& operator<<(std::ostream& out, const IncrData& incrData) {
        if(!incrData.expr) return out;
        return out << *incrData.expr;
    }
};

class IncrPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    IncrData                          data;
    std::size_t                       incrDepth;
    std::unique_ptr<ExpressionPolicy> exprPolicy;

public:
    IncrPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          incrDepth(0) {
        InitializeIncrPolicyHandlers();
    }

    ~IncrPolicy() {}

protected:
    std::any DataInner() const override { return std::make_shared<IncrData>(data); }

    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        if (typeid(ExpressionPolicy) == typeid(*policy)) {
            data.expr = policy->Data<ExpressionData>();
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeIncrPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {                     
            if (!incrDepth) {                          
                incrDepth = ctx.depth;                 
                data = IncrData{};                     
                data.startLineNumber = ctx.startLineNumber; 
                CollectExpressionHandlers();                       
            }                                                 
        }; 

        // end of policy
        closeEventMap[ParserState::incr] =[this](srcSAXEventContext& ctx) {
            if (incrDepth && incrDepth == ctx.depth) {
                incrDepth = 0;
                NotifyAll(ctx);
                InitializeIncrPolicyHandlers();
            }
        };
    }

    void CollectExpressionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if(!incrDepth) return;
            if((incrDepth + 1) != ctx.depth) return;

            if (!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(exprPolicy.get());                
        };
    }
};

#endif
