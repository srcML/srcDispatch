/**
 * @file ControlPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_CONTROL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CONTROL_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <DeclTypePolicySingleEvent.hpp>
#include <ConditionPolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ControlData {

    unsigned int lineNumber;

    std::shared_ptr<DeclTypeData>   init;
    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<ExpressionData> incr;

    friend std::ostream& operator<<(std::ostream& out, const ControlData& controlData) {
        if(controlData.init)      out << controlData.init      << "; ";
        if(controlData.condition) out << controlData.condition << "; ";
        if(controlData.incr)      out << controlData.incr;
        return out;
    }
};

// Collect the expression in the return
//
class ControlPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ControlData        data;
    std::size_t        controlDepth;
    DeclTypePolicy  *  declPolicy;
    ConditionPolicy *  conditionPolicy;
    ExpressionPolicy*  exprPolicy;

public:
    ControlPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          controlDepth(0),
          exprPolicy(nullptr) {
        InitializeControlPolicyHandlers();
    }

    ~ControlPolicy() {
        if (declPolicy)      delete declPolicy;
        if (conditionPolicy) delete conditionPolicy;
        if (exprPolicy)      delete exprPolicy;
    }

protected:
    std::any DataInner() const override { return std::make_shared<ControlData>(data); }

    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        using namespace srcDispatch;
        if(typeid(DeclTypePolicy) == typeid(*policy)) {
            data.init = policy->Data<DeclTypeData>();
        } else if(typeid(ConditionPolicy) == typeid(*policy)) {
            data.condition = policy->Data<ExpressionData>();
        }  else if(typeid(ExpressionPolicy) == typeid(*policy)) {
            if(ctx.IsOpen(ParserState::init)) {
                std::shared_ptr<ExpressionData> expr = policy->Data<ExpressionData>();
                std::shared_ptr<DeclTypeData>   decl = std::make_shared<DeclTypeData>(expr->lineNumber);
                decl->initializer = expr;
                data.init         = decl;
            } else {
                data.incr      = policy->Data<ExpressionData>();
            }
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) override {} //doesn't use other parsers

private:
    void InitializeControlPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
            if (!controlDepth) {
                controlDepth = ctx.depth;
                data = ControlData{};
                CollectInitHandlers();
                CollectConditionHandlers();
                CollectIncrHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
            if (controlDepth && controlDepth == ctx.depth) {
                controlDepth = 0;
                NotifyAll(ctx);
                InitializeControlPolicyHandlers();
            }
        };
    }

    void CollectInitHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;
            // if (!declPolicy) declPolicy = new DeclTypePolicy{this};
            // ctx.dispatcher->AddListenerDispatch(declPolicy);
        };
    }

    void CollectConditionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;

            if (!conditionPolicy) conditionPolicy = new ConditionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(conditionPolicy);
        };
    }

    void CollectIncrHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;
            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if (!exprPolicy) exprPolicy = new ExpressionPolicy{this};
                ctx.dispatcher->AddListenerDispatch(exprPolicy);
            };
        };
        closeEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;
            NopOpenEvents({ParserState::expr});
        };
    }

};

#endif
