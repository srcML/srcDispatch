/**
 * @file IfStmtPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_IF_STMT_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_IF_STMT_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <IfPolicySingleEvent.hpp>
#include <ElseIfPolicySingleEvent.hpp>
#include <ElsePolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct IfStmtData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::vector<std::any> clauses;

    friend std::ostream& operator<<(std::ostream& out, const IfStmtData& ifStmtData) {
        if(ifStmtData.clauses.empty())                                           return out;
        if(ifStmtData.clauses.front().type() != typeid(std::shared_ptr<IfData>)) return out;
        return out << *std::any_cast<std::shared_ptr<IfData>>(ifStmtData.clauses.front());
    }
};

class IfStmtPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    IfStmtData     data;
    std::size_t    ifStmtDepth;
    std::unique_ptr<IfPolicy>     ifPolicy;
    std::unique_ptr<ElseIfPolicy> elseIfPolicy;
    std::unique_ptr<ElsePolicy>   elsePolicy;

public:
    IfStmtPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          ifStmtDepth(0) {
        InitializeIfStmtPolicyHandlers();
    }

    ~IfStmtPolicy() {}

protected:
    std::any DataInner() const { return std::make_shared<IfStmtData>(data); }

    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if (typeid(IfPolicy) == typeid(*policy)) {
            data.clauses.push_back(policy->Data<IfData>());
        } else if (typeid(ElseIfPolicy) == typeid(*policy)) {
            data.clauses.push_back(policy->Data<ElseIfData>());
        } else if (typeid(ElsePolicy) == typeid(*policy)) {
            data.clauses.push_back(policy->Data<ElseData>());
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {} //doesn't use other parsers

private:
    void InitializeIfStmtPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::ifgroup] = [this](srcSAXEventContext& ctx) {                     
            if (!ifStmtDepth) {                          
                ifStmtDepth = ctx.depth;                 
                data = IfStmtData{};                     
                data.startLineNumber = ctx.startLineNumber; 
                CollectIfHandlers();                   
                CollectElseIfHandlers();                 
                CollectElseHandlers();                       
            }                                                 
        };

        // end of policy
        closeEventMap[ParserState::ifgroup] =[this](srcSAXEventContext& ctx) {
            if (ifStmtDepth && ifStmtDepth == ctx.depth) {
                ifStmtDepth = 0;
                data.endLineNumber = ctx.startLineNumber;
                NotifyAll(ctx);
                InitializeIfStmtPolicyHandlers();
            }
        };
    }

    void CollectIfHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
            if(!ifStmtDepth) return;
            if((ifStmtDepth + 1) != ctx.depth) return;

            if (!ifPolicy) ifPolicy = make_unique_policy<IfPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(ifPolicy.get());
        };              
    }

    void CollectElseIfHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::elseif] = [this](srcSAXEventContext& ctx) {
            if(!ifStmtDepth) return;
            if((ifStmtDepth + 1) != ctx.depth) return;

            if (!elseIfPolicy) elseIfPolicy = make_unique_policy<ElseIfPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(elseIfPolicy.get());
        };
    }

    void CollectElseHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::elsestmt] = [this](srcSAXEventContext& ctx) {
            if(!ifStmtDepth) return;
            if((ifStmtDepth + 1) != ctx.depth) return;

            if (!elsePolicy) elsePolicy = make_unique_policy<ElsePolicy>({this});
            ctx.dispatcher->AddListenerDispatch(elsePolicy.get());
        };
    }
};

#endif
