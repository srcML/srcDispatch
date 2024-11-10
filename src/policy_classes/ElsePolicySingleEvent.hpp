/**
 * @file ElsePolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_ELSE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_ELSE_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ConditionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ElseData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;

    friend std::ostream& operator<<(std::ostream& out, const ElseData& elseData) {
        if(!elseData.condition) return out;
        return out << *elseData.condition;
    }
};

class ElsePolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ElseData  data;
    std::size_t      elseDepth;
    ConditionPolicy* conditionPolicy;
    BlockPolicy    * blockPolicy;

public:
    ElsePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          elseDepth(0),
          conditionPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeElsePolicyHandlers();
    }

    ~ElsePolicy() {
        if (conditionPolicy) delete conditionPolicy;
        if (blockPolicy)     delete blockPolicy;
    }

protected:
    std::any DataInner() const { return std::make_shared<ElseData>(data); }

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
    void InitializeElsePolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::elsestmt] = [this](srcSAXEventContext& ctx) {                     
            if (!elseDepth) {                          
                elseDepth = ctx.depth;                 
                data = ElseData{};                     
                data.startLineNumber = ctx.currentLineNumber; 
                CollectBlockHandlers();                       
            }                                                 
        }; 

        // end of policy
        closeEventMap[ParserState::elsestmt] =[this](srcSAXEventContext& ctx) {
            if (elseDepth && elseDepth == ctx.depth) {
                elseDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeElsePolicyHandlers();
            }
        };
    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!elseDepth) return;
            if((elseDepth + 1) != ctx.depth) return;

            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        };
    }
};

#endif
