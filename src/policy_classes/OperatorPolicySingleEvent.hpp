/**
 * @file OperatorPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_OPERATOR_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_OPERATOR_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <string>
#include <iostream>

struct OperatorData {

    unsigned int startLineNumber;
    std::string op;

    friend std::ostream& operator<<(std::ostream& out, const OperatorData& operatorData) {
        return out << operatorData.op;
    }
};

class OperatorPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    OperatorData data;
    std::size_t  operatorDepth;

public:
    OperatorPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          operatorDepth(0) {
        InitializeOperatorPolicyHandlers();
    }

    ~OperatorPolicy() {
    }

protected:
    std::any DataInner() const { return std::make_shared<OperatorData>(data); }

    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {}

    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {}

private:
    void InitializeOperatorPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::op] = [this](srcSAXEventContext& ctx) {                     
            if (!operatorDepth) {                          
                operatorDepth = ctx.depth;                 
                data = OperatorData{};                     
                data.startLineNumber = ctx.currentLineNumber; 
                CollectTokenHandlers();                       
            }                                                 
        }; 

        // end of policy
        closeEventMap[ParserState::op] =[this](srcSAXEventContext& ctx) {
            if (operatorDepth && operatorDepth == ctx.depth) {
                operatorDepth = 0;
                NotifyAll(ctx);
                InitializeOperatorPolicyHandlers();
            }
        };
    }

    void CollectTokenHandlers() {
        using namespace srcDispatch;
        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
            data.op += ctx.currentToken;
        };
    }
};

#endif
