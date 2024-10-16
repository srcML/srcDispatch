/**
 * @file ConditionalPolicySingleEvent.cpp
 *
 *
 */

#include <ConditionalPolicySingleEvent.hpp>

#include <BlockPolicySingleEvent.hpp>

ConditionalPolicy::ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      conditionalDepth(0),
      exprPolicy(nullptr) {
    InitializeConditionalPolicyHandlers();
}

ConditionalPolicy::~ConditionalPolicy() {
    if (exprPolicy) delete exprPolicy;
}

std::any ConditionalPolicy::DataInner() const { return std::make_shared<ConditionalData>(data); }

void ConditionalPolicy::Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) {
    if (typeid(BlockPolicy) == typeid(*policy)) {
        data.block = policy->Data<BlockData>();
        ctx.dispatcher->RemoveListener(nullptr);
    }
}

void ConditionalPolicy::NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) {} //doesn't use other parsers

void ConditionalPolicy::InitializeConditionalPolicyHandlers() {
    using namespace srcDispatch;

    #define startConditional(TYPE)        \
    [this](srcSAXEventContext& ctx) {     \
        if (!conditionalDepth) {          \
            conditionalDepth = ctx.depth; \
            data = ConditionalData{};     \
            data.type = TYPE;             \
            CollectConditionHandlers();   \
            CollectBlockHandlers();       \
        }                                 \
    };                                    \

    openEventMap[ParserState::ifstmt]     = startConditional(ConditionalData::IF);
    openEventMap[ParserState::whilestmt]  = startConditional(ConditionalData::WHILE);
    openEventMap[ParserState::forstmt]    = startConditional(ConditionalData::FOR);
    openEventMap[ParserState::switchstmt] = startConditional(ConditionalData::SWITCH);
    openEventMap[ParserState::dostmt]     = startConditional(ConditionalData::DO);

    std::function<void (srcSAXEventContext& ctx)> endConditional =[this](srcSAXEventContext& ctx) {
        if (conditionalDepth && conditionalDepth == ctx.depth) {
            conditionalDepth = 0;
            NotifyAll(ctx);
            InitializeConditionalPolicyHandlers();
        }
    };

    // end of policy
    closeEventMap[ParserState::ifstmt]     = endConditional;
    closeEventMap[ParserState::whilestmt]  = endConditional;
    closeEventMap[ParserState::forstmt]    = endConditional;
    closeEventMap[ParserState::switchstmt] = endConditional;
    closeEventMap[ParserState::dostmt]     = endConditional;
}

void ConditionalPolicy::CollectConditionHandlers() {
}

void ConditionalPolicy::CollectBlockHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
        if (conditionalDepth && (conditionalDepth + 1) == ctx.depth) {
            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        }
    };

}
