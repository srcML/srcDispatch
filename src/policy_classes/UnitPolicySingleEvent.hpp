/**
 * Policy for srcDispatch
 * Listens for both classes and functions
 * Calls the ClassPolicySingleEvent for class
 * Calls the FunctionPolicySingleEvent for function
 *
 */
#ifndef INCLUDED_UNIT_POLICY_HPP
#define INCLUDED_UNIT_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <DeclTypePolicySingleEvent.hpp>
#include <FunctionPolicySingleEvent.hpp>
#include <ClassPolicySingleEvent.hpp>

#include <typeinfo>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>


class UnitPolicy : 
	public srcDispatch::EventListener,
	public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener   {

public:
    DeclTypePolicy* declPolicy;
    FunctionPolicy* functionPolicy;
    ClassPolicy   * classPolicy;

public:
    UnitPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners) :
                srcDispatch::PolicyDispatcher(listeners),
                functionPolicy(nullptr),
                classPolicy(nullptr) {
        InitializeUnitPolicyHandlers();
    }

    ~UnitPolicy() {
        if(declPolicy)     delete declPolicy;
        if(functionPolicy) delete functionPolicy;
        if(classPolicy)    delete classPolicy;
    }

    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        // Assumes at least one lister which should always be one
        policyListeners.back()->Notify(policy, ctx);
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

protected:
    std::any DataInner() const override { return std::any(); }

private:
    void InitializeUnitPolicyHandlers() {
        using namespace srcDispatch;

        // start of policy
        std::function<void(srcDispatch::srcSAXEventContext&)> startClassPolicy = [this](srcSAXEventContext& ctx) {
            if(!classPolicy) classPolicy = new ClassPolicy{this};
            ctx.dispatcher->AddListenerDispatch(classPolicy);
        };

        openEventMap[ParserState::classn]   = startClassPolicy;
        openEventMap[ParserState::structn]  = startClassPolicy;

        // end of policy
        std::function<void(srcDispatch::srcSAXEventContext&)> endClassPolicy = [](srcSAXEventContext& ctx) {
        };

        closeEventMap[ParserState::classn]  = endClassPolicy;
        closeEventMap[ParserState::structn] = endClassPolicy;

        // start function of policy
        std::function<void(srcDispatch::srcSAXEventContext&)> startFunction = [this](srcSAXEventContext& ctx) {
            if(!functionPolicy) functionPolicy = new FunctionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(functionPolicy);
        };


        openEventMap[ParserState::function]    = startFunction;
        openEventMap[ParserState::constructor] = startFunction;
        openEventMap[ParserState::destructor]  = startFunction;

        // end of policy
        std::function<void(srcDispatch::srcSAXEventContext&)> endFunction = [](srcSAXEventContext& ctx) {
        };


        closeEventMap[ParserState::function]    = endFunction;
        closeEventMap[ParserState::constructor] = endFunction;
        closeEventMap[ParserState::destructor]  = endFunction;

        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if(!declPolicy) declPolicy = new DeclTypePolicy{this};
            ctx.dispatcher->AddListenerDispatch(declPolicy);
        };

        closeEventMap[ParserState::declstmt] = [](srcSAXEventContext& ctx) {
        };

    }
};

#endif
