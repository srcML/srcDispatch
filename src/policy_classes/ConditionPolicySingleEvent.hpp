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
#include <DeclTypePolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ConditionData {
    std::shared_ptr<ExpressionData>        expr;
    std::vector<std::shared_ptr<DeclData>> decls;

    friend std::ostream& operator<<(std::ostream& out, const ConditionData& condition) {
        bool outputDecl = false;
        for(const std::shared_ptr<DeclData>& decl : condition.decls) {
            if(outputDecl) out << ", ";
            out << *decl;
            outputDecl = true;
        }

        if(outputDecl) out << "; ";
        out << *condition.expr;

        return out;
    }
};

// Collect the expression in the return
//
class ConditionPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ConditionData                     data;
    std::size_t                       conditionDepth;
    std::unique_ptr<ExpressionPolicy> exprPolicy;
    std::unique_ptr<DeclTypePolicy>   declTypePolicy;
public:
    ConditionPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          conditionDepth(0) {
        InitializeConditionPolicyHandlers();
    }

    ~ConditionPolicy() {}

protected:
    std::any DataInner() const override { return std::make_shared<ConditionData>(data); }

    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        if (typeid(ExpressionPolicy) == typeid(*policy)) {
            data.expr = policy->Data<ExpressionData>();
        } else if (typeid(DeclTypePolicy) == typeid(*policy)) {
           data.decls = *policy->Data<std::vector<std::shared_ptr<DeclData>>>();
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeConditionPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if (!conditionDepth) {
                conditionDepth = ctx.depth;
                data = ConditionData{};
                CollectExpressionHandlers();
                CollectDeclTypePolicyHandlers();
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
            if (!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
        };
    }

    void CollectDeclTypePolicyHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if (!declTypePolicy) declTypePolicy = make_unique_policy<DeclTypePolicy>({this});
            ctx.dispatcher->AddListenerDispatch(declTypePolicy.get());
        };
    }

};

#endif
