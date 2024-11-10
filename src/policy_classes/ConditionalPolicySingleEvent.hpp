/**
 * @file ConditionalPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_CONDITIONAL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CONDITIONAL_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ConditionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ConditionalData {

    enum ConditionalType { IF, WHILE, SWITCH, DO };

    ConditionalType type;

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;

    friend std::ostream& operator<<(std::ostream& out, const ConditionalData& conditionalData) {
        if(!conditionalData.condition) return out;
        return out << *conditionalData.condition;
    }
};


class ConditionalPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ConditionalData  data;
    std::size_t      conditionalDepth;
    ConditionPolicy* conditionPolicy;
    BlockPolicy    * blockPolicy;

public:
    ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          conditionalDepth(0),
          conditionPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeConditionalPolicyHandlers();
    }

    ~ConditionalPolicy() {
        if (conditionPolicy) delete conditionPolicy;
        if (blockPolicy)     delete blockPolicy;
    }

protected:
    std::any DataInner() const { return std::make_shared<ConditionalData>(data); }

    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if (typeid(ConditionPolicy) == typeid(*policy)) {
            data.condition = policy->Data<ExpressionData>();
        }else if (typeid(BlockPolicy) == typeid(*policy)) {
            data.block = policy->Data<BlockData>();
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {} //doesn't use other parsers

private:
    void InitializeConditionalPolicyHandlers() {
        using namespace srcDispatch;

        #define startConditional(TYPE)                        \
        [this](srcSAXEventContext& ctx) {                     \
            if (!conditionalDepth) {                          \
                conditionalDepth = ctx.depth;                 \
                data = ConditionalData{};                     \
                data.type = TYPE;                             \
                data.startLineNumber = ctx.currentLineNumber; \
                CollectConditionHandlers();                   \
                CollectBlockHandlers();                       \
            }                                                 \
        };                                                    \

        openEventMap[ParserState::ifstmt]     = startConditional(ConditionalData::IF);
        openEventMap[ParserState::whilestmt]  = startConditional(ConditionalData::WHILE);
        openEventMap[ParserState::switchstmt] = startConditional(ConditionalData::SWITCH);
        openEventMap[ParserState::dostmt]     = startConditional(ConditionalData::DO);

        std::function<void (srcSAXEventContext& ctx)> endConditional =[this](srcSAXEventContext& ctx) {
            if (conditionalDepth && conditionalDepth == ctx.depth) {
                conditionalDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeConditionalPolicyHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::ifstmt]     = endConditional;
        closeEventMap[ParserState::whilestmt]  = endConditional;
        closeEventMap[ParserState::switchstmt] = endConditional;
        closeEventMap[ParserState::dostmt]     = endConditional;
    }

    void CollectConditionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if(!conditionalDepth) return;
            if((conditionalDepth + 1) != ctx.depth) return;

            if (!conditionPolicy) conditionPolicy = new ConditionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(conditionPolicy);  
        };              
    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!conditionalDepth) return;
            if((conditionalDepth + 1) != ctx.depth) return;

            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        };
    }
};

#endif
