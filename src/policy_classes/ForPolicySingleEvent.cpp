/**
 * @file ForPolicySingleEvent.cpp
 *
 *
 */

#include <ForPolicySingleEvent.hpp>

#include <BlockPolicySingleEvent.hpp>

std::ostream& operator<<(std::ostream& out, const ForData& conditionalData) {
    if(!conditionalData.control) return out;
    return out << conditionalData.control;
}

ForPolicy::ForPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      conditionalDepth(0),
      controlPolicy(nullptr),
      blockPolicy(nullptr) {
    InitializeForPolicyHandlers();
}

ForPolicy::~ForPolicy() {
    if (controlPolicy)   delete controlPolicy;
    if (blockPolicy)     delete blockPolicy;
}

std::any ForPolicy::DataInner() const { return std::make_shared<ForData>(data); }

void ForPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    if (typeid(ControlPolicy) == typeid(*policy)) {
        data.control = policy->Data<ControlData>();
    } else if (typeid(BlockPolicy) == typeid(*policy)) {
        data.block = policy->Data<BlockData>();
    } else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListener(nullptr);
}

void ForPolicy::NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {} //doesn't use other parsers

void ForPolicy::InitializeForPolicyHandlers() {
    using namespace srcDispatch;

    openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {                     \
        if (!conditionalDepth) {
            conditionalDepth = ctx.depth;
            data = ForData{};
            data.startLineNumber = ctx.currentLineNumber;
            CollectControlHandlers();
            CollectBlockHandlers();
        } 
    };

    // end of policy
    closeEventMap[ParserState::forstmt] =[this](srcSAXEventContext& ctx) {
        if (conditionalDepth && conditionalDepth == ctx.depth) {
            conditionalDepth = 0;
            data.endLineNumber = ctx.currentLineNumber;
            NotifyAll(ctx);
            InitializeForPolicyHandlers();
        }
    };
}

void ForPolicy::CollectControlHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
        if(!conditionalDepth) return;
        if((conditionalDepth + 1) != ctx.depth) return;

        if (!controlPolicy) controlPolicy = new ControlPolicy{this};
        ctx.dispatcher->AddListenerDispatch(controlPolicy);  
    };              
}

void ForPolicy::CollectBlockHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
        if(conditionalDepth && (conditionalDepth + 1) == ctx.depth) {
            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        }
    };
}
