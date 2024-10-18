/**
 * @file ConditionalPolicySingleEvent.cpp
 *
 *
 */

#include <ConditionalPolicySingleEvent.hpp>

#include <BlockPolicySingleEvent.hpp>

std::ostream & operator<<(std::ostream& out, const ConditionalData& conditionalData) {
    if (conditionalData.condition){
        out << *conditionalData.condition;
    }
    return out;
}

ConditionalPolicy::ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      conditionalDepth(0),
      conditionPolicy(nullptr),
      blockPolicy(nullptr) {
    InitializeConditionalPolicyHandlers();
}

ConditionalPolicy::~ConditionalPolicy() {
    if (conditionPolicy) delete conditionPolicy;
    if (blockPolicy)     delete blockPolicy;
}

std::any ConditionalPolicy::DataInner() const { return std::make_shared<ConditionalData>(data); }

void ConditionalPolicy::Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) {
    if (typeid(ConditionPolicy) == typeid(*policy)) {
        data.condition = policy->Data<ExpressionData>();
    } else if (typeid(BlockPolicy) == typeid(*policy)) {
        data.block = policy->Data<BlockData>();
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListener(nullptr);
}

void ConditionalPolicy::NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) {} //doesn't use other parsers

void ConditionalPolicy::InitializeConditionalPolicyHandlers() {
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
    openEventMap[ParserState::forstmt]    = startConditional(ConditionalData::FOR);
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
    closeEventMap[ParserState::forstmt]    = endConditional;
    closeEventMap[ParserState::switchstmt] = endConditional;
    closeEventMap[ParserState::dostmt]     = endConditional;
}

void ConditionalPolicy::CollectConditionHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
        if(!conditionalDepth) return;
        if(data.type == ConditionalData::FOR && (conditionalDepth + 2) != ctx.depth) return;
        if(data.type != ConditionalData::FOR && (conditionalDepth + 1) != ctx.depth) return;

        if (!conditionPolicy) conditionPolicy = new ConditionPolicy{this};
        ctx.dispatcher->AddListenerDispatch(conditionPolicy);  
    };              
}

void ConditionalPolicy::CollectBlockHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
        if(conditionalDepth && (conditionalDepth + 1) == ctx.depth) {
            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        }
    };
}
