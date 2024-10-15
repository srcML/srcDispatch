/**
 * @file ConditionalPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_CONDITIONAL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CONDITIONAL_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatchSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ExpressionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ConditionalData {

    enum ConditionalType { IF, WHILE, FOR, FOREACH, SWITCH, DO };

    ConditionalType type;

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;
};


class ConditionalPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ConditionalData   data;
    std::size_t       conditionalDepth;
    ExpressionPolicy* exprPolicy;
    BlockPolicy     * blockPolicy;

public:
    ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          returnDepth(0),
          exprPolicy(nullptr) {
        InitializeConditionalPolicyHandlers();
    }

    ~ConditionalPolicy() {
        if (exprPolicy) delete exprPolicy;
    }

protected:
    std::any DataInner() const override { return std::make_shared<ConditionalData>(data); }

    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override {
        if (typeid(BlockPolicy) == typeid(*policy)) {
            data = policy->Data<BlockData>();
            ctx.dispatcher->RemoveListener(nullptr);
        }
    }

    void NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

private:
    void InitializeConditionalPolicyHandlers() {
        using namespace srcDispatch;

        // start of policy
        openEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
            if (!conditionalDepth) {
                conditionalDepth = ctx.depth;
                data = ConditionalData{};
                data.type = IF;
                CollectConditionHandlers();
                CollectBlockHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
            if (conditionalDepth && conditionalDepth == ctx.depth) {
                conditionalDepth = 0;
                NotifyAll(ctx);
                InitializeConditionalPolicyHandlers();
            }
        };
    }

    void CollectConditionHandlers() {
    }

    void CollectBlockHandlers() {

        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if (conditionalDepth && (conditionalDepth + 1) == ctx.depth) {
                if (!blockPolicy) blockPolicy = new BlockPolicy{this};
                ctx.dispatcher->AddListenerDispatch(blockPolicy);                
            }
        };

    }

};

#endif
