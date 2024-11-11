/**
 * @file ForPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_FOR_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_FOR_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ControlPolicySingleEvent.hpp>
#include <BlockPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

class BlockPolicy;
class BlockData;

struct ForData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ControlData> control;
    std::shared_ptr<BlockData>   block;

    friend std::ostream& operator<<(std::ostream& out, const ForData& conditionalData) {
        if(!conditionalData.control) return out;
        return out << *conditionalData.control;
    }
};


class ForPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ForData         data;
    std::size_t     conditionalDepth;
    ControlPolicy * controlPolicy;
    BlockPolicy   * blockPolicy;

public:

    ForPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          conditionalDepth(0),
          controlPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeForPolicyHandlers();
    }

    ~ForPolicy() {
        if (controlPolicy) delete controlPolicy;
        if (blockPolicy)   delete blockPolicy;
    }

protected:
    std::any DataInner() const { return std::make_shared<ForData>(data); }

    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if (typeid(ControlPolicy) == typeid(*policy)) {
            data.control = policy->Data<ControlData>();
        } else if (typeid(BlockPolicy) == typeid(*policy)) {
            data.block = policy->Data<BlockData>();
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {} //doesn't use other parsers

private:

    void InitializeForPolicyHandlers() {
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


    void CollectControlHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
            if(!conditionalDepth) return;
            if((conditionalDepth + 1) != ctx.depth) return;

            if (!controlPolicy) controlPolicy = new ControlPolicy{this};
            ctx.dispatcher->AddListenerDispatch(controlPolicy);  
        };              
    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(conditionalDepth && (conditionalDepth + 1) == ctx.depth) {
                if (!blockPolicy) blockPolicy = new BlockPolicy{this};
                ctx.dispatcher->AddListenerDispatch(blockPolicy);                
            }
        };
    }
};

#endif
