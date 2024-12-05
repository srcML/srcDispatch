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

#include <DeclPolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>
#include <ConditionPolicySingleEvent.hpp>
#include <IncrPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ControlData {

    unsigned int lineNumber;

    std::vector<std::any>                        init;
    std::shared_ptr<ExpressionData>              condition;
    std::vector<std::shared_ptr<ExpressionData>> incr;

    friend std::ostream& operator<<(std::ostream& out, const ControlData& controlData) {

        bool outputDeclComma = false;
        for(const std::any& item : controlData.init) {
            if(outputDeclComma) out << ", ";
            if(item.type() == typeid(std::shared_ptr<DeclData>))
                out << *std::any_cast<std::shared_ptr<DeclData>>(item);
            else {
                out << *std::any_cast<std::shared_ptr<ExpressionData>>(item);
            }
            outputDeclComma = true;
        }

        if(controlData.condition) out << *controlData.condition << "; ";

        bool outputExprComma = false;
        for(const std::shared_ptr<ExpressionData>& expr : controlData.incr) {
            if(outputExprComma) out << ", ";
            out << *expr;
            outputExprComma = true;
        }
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
    ControlData                        data;
    std::size_t                        controlDepth;
    std::unique_ptr<DeclPolicy>        declPolicy;
    std::unique_ptr<ExpressionPolicy>  exprPolicy;
    std::unique_ptr<ConditionPolicy>   conditionPolicy;
    std::unique_ptr<IncrPolicy>        incrPolicy;

public:
    ControlPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          controlDepth(0) {
        InitializeControlPolicyHandlers();
    }

    ~ControlPolicy() {}

protected:
    std::any DataInner() const override { return std::make_shared<ControlData>(data); }

    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        using namespace srcDispatch;
        if(typeid(DeclPolicy) == typeid(*policy)) {
            data.init.push_back(policy->Data<DeclData>());
        } else if(typeid(ExpressionPolicy) == typeid(*policy)) {
            data.init.push_back(policy->Data<ExpressionData>());
        } else if(typeid(ConditionPolicy) == typeid(*policy)) {
            data.condition = policy->Data<ExpressionData>();
        } else if(typeid(IncrPolicy) == typeid(*policy)) {
            data.incr.push_back(policy->Data<ExpressionData>());
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

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

            openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
                if(ctx.depth != (controlDepth + 2)) return;

                if (!declPolicy) declPolicy = make_unique_policy<DeclPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(declPolicy.get());
            };
            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if(ctx.depth != (controlDepth + 2)) return;

                if (!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        };
        closeEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;
            NopOpenEvents({ParserState::decl, ParserState::expr});
        };

    }

    void CollectConditionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;

            if (!conditionPolicy) conditionPolicy = make_unique_policy<ConditionPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(conditionPolicy.get());
        };
    }

    void CollectIncrHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;
            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if (!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        };
        closeEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
            if(ctx.depth != (controlDepth + 1)) return;
            NopOpenEvents({ParserState::expr});
        };
    }

};

#endif
