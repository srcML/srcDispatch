/**
 * @file DoPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_DO_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DO_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ConditionPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct DoData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;

    friend std::ostream& operator<<(std::ostream& out, const DoData& doData) {
        if(!doData.condition) return out;
        return out << *doData.condition;
    }
};

class DoPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    DoData  data;
    std::size_t      doDepth;
    ConditionPolicy* conditionPolicy;
    BlockPolicy    * blockPolicy;

public:
    DoPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          doDepth(0),
          conditionPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeDoPolicyHandlers();
    }

    ~DoPolicy() {
        if (conditionPolicy) delete conditionPolicy;
        if (blockPolicy)     delete blockPolicy;
    }

protected:
    std::any DataInner() const { return std::make_shared<DoData>(data); }

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
    void InitializeDoPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::dostmt] = [this](srcSAXEventContext& ctx) {                     
            if (!doDepth) {                          
                doDepth = ctx.depth;                 
                data = DoData{};                     
                data.startLineNumber = ctx.currentLineNumber; 
                CollectConditionHandlers();                   
                CollectBlockHandlers();                       
            }                                                 
        };   

        // end of policy
        closeEventMap[ParserState::dostmt] =[this](srcSAXEventContext& ctx) {
            if (doDepth && doDepth == ctx.depth) {
                doDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeDoPolicyHandlers();
            }
        };
    }

    void CollectConditionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
            if(!doDepth) return;
            if((doDepth + 1) != ctx.depth) return;

            if (!conditionPolicy) conditionPolicy = new ConditionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(conditionPolicy);  
        };              
    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!doDepth) return;
            if((doDepth + 1) != ctx.depth) return;

            if (!blockPolicy) blockPolicy = new BlockPolicy{this};
            ctx.dispatcher->AddListenerDispatch(blockPolicy);                
        };
    }
};

#endif
