/**
 * @file ElseIfPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_ELSEIF_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_ELSEIF_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ConditionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ElseIfData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;

    friend std::ostream& operator<<(std::ostream& out, const ElseIfData& elseIfData) {
        if(!elseIfData.condition) return out;
        return out << *elseIfData.condition;
    }
};

class ElseIfPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ElseIfData  data;
    std::size_t      elseIfDepth;
    ConditionPolicy* conditionPolicy;
    BlockPolicy    * blockPolicy;

public:
    ElseIfPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          elseIfDepth(0),
          conditionPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeElseIfPolicyHandlers();
    }

    ~ElseIfPolicy() {
        if (conditionPolicy) delete conditionPolicy;
        if (blockPolicy)     delete blockPolicy;
    }

protected:
    std::any DataInner() const { return std::make_shared<ElseIfData>(data); }

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
    void InitializeElseIfPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::elseif] = [this](srcSAXEventContext& ctx) {                     
            if (!elseIfDepth) {                          
                elseIfDepth = ctx.depth;                 
                data = ElseIfData{};                     
                data.startLineNumber = ctx.currentLineNumber; 
                CollectConditionHandlers();                   
                CollectBlockHandlers();                       
            }                                                 
        }; 

        // end of policy
        closeEventMap[ParserState::elseif] =[this](srcSAXEventContext& ctx) {
            if (elseIfDepth && elseIfDepth == ctx.depth) {
                elseIfDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeElseIfPolicyHandlers();
            }
        };
    }

    void CollectConditionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if(!elseIfDepth) return;
            if((elseIfDepth + 1) != ctx.depth) return;

            if (!conditionPolicy) conditionPolicy = new ConditionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(conditionPolicy);  
        };              
    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!elseIfDepth) return;
            if((elseIfDepth + 1) != ctx.depth) return;

            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        };
    }
};

#endif
