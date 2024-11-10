/**
 * @file SwitchPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_SWITCH_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_SWITCH_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ConditionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct SwitchData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;

    friend std::ostream& operator<<(std::ostream& out, const SwitchData& switchData) {
        if(!switchData.condition) return out;
        return out << *switchData.condition;
    }
};

class SwitchPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    SwitchData  data;
    std::size_t      switchDepth;
    ConditionPolicy* conditionPolicy;
    BlockPolicy    * blockPolicy;

public:
    SwitchPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          switchDepth(0),
          conditionPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeSwitchPolicyHandlers();
    }

    ~SwitchPolicy() {
        if (conditionPolicy) delete conditionPolicy;
        if (blockPolicy)     delete blockPolicy;
    }

protected:
    std::any DataInner() const { return std::make_shared<SwitchData>(data); }

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
    void InitializeSwitchPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::switchstmt] = [this](srcSAXEventContext& ctx) {                     
            if (!switchDepth) {                          
                switchDepth = ctx.depth;                 
                data = SwitchData{};                     
                data.startLineNumber = ctx.currentLineNumber; 
                CollectConditionHandlers();                   
                CollectBlockHandlers();                       
            }                                                 
        };   

        std::function<void (srcSAXEventContext& ctx)> endConditional =[this](srcSAXEventContext& ctx) {
            if (switchDepth && switchDepth == ctx.depth) {
                switchDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeSwitchPolicyHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::switchstmt] =[this](srcSAXEventContext& ctx) {
            if (switchDepth && switchDepth == ctx.depth) {
                switchDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeSwitchPolicyHandlers();
            }
        };
    }

    void CollectConditionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if(!switchDepth) return;
            if((switchDepth + 1) != ctx.depth) return;

            if (!conditionPolicy) conditionPolicy = new ConditionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(conditionPolicy);  
        };              
    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!switchDepth) return;
            if((switchDepth + 1) != ctx.depth) return;

            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        };
    }
};

#endif
