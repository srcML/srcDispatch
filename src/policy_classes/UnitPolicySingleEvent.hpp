/**
 * Policy for srcSAXEventDispatcher
 * Listens for both classes and functions
 * Calls the ClassPolicySingleEvent for class
 * Calls the FunctionPolicySingleEvent for function
 *
 */
#ifndef INCLUDED_UNIT_POLICY_HPP
#define INCLUDED_UNIT_POLICY_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <FunctionPolicySingleEvent.hpp>
#include <ClassPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>


class UnitPolicySingleEvent : 
	public srcSAXEventDispatch::EventListener,
	public srcSAXEventDispatch::PolicyDispatcher,
    public srcSAXEventDispatch::PolicyListener   {

public:
    FunctionPolicy *functionPolicy;
    ClassPolicy    *classPolicy;

public:
    UnitPolicySingleEvent(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners) :
                srcSAXEventDispatch::PolicyDispatcher(listeners),
                functionPolicy(nullptr),
                classPolicy(nullptr) {
        InitializeUnitPolicyHandlers();
    }

    ~UnitPolicySingleEvent() {
        if(functionPolicy) delete functionPolicy;
        if(classPolicy)    delete classPolicy;
    }

    void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

    void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
        // Assumes at least one lister which should always be one
        policyListeners.back()->Notify(policy, ctx);
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    }

protected:
    std::any DataInner() const override { return std::any(); }

private:
    void InitializeUnitPolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> startClassPolicy = [this](srcSAXEventContext& ctx) {
            if(!classPolicy) classPolicy = new ClassPolicy{this};
            ctx.dispatcher->AddListenerDispatch(classPolicy);
        };

        // end of policy
        std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> endClassPolicy = [this](srcSAXEventContext& ctx) {
        };

        openEventMap[ParserState::classn] = startClassPolicy;
        closeEventMap[ParserState::classn] = endClassPolicy;
        openEventMap[ParserState::structn] = startClassPolicy;
        closeEventMap[ParserState::structn] = endClassPolicy;

        // start function of policy
        std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> startFunction = [this](srcSAXEventContext& ctx) {
            if(!functionPolicy) functionPolicy = new FunctionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(functionPolicy);
        };

        // end of policy
        std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> endFunction = [this](srcSAXEventContext& ctx) {
        };

        openEventMap[ParserState::function]        = startFunction;
        openEventMap[ParserState::functiondecl]    = startFunction;
        openEventMap[ParserState::constructor]     = startFunction;
        openEventMap[ParserState::constructordecl] = startFunction;
        openEventMap[ParserState::destructor]      = startFunction;
        openEventMap[ParserState::destructordecl]  = startFunction;

        closeEventMap[ParserState::function]        = endFunction;
        closeEventMap[ParserState::functiondecl]    = endFunction;
        closeEventMap[ParserState::constructor]     = endFunction;
        closeEventMap[ParserState::constructordecl] = endFunction;
        closeEventMap[ParserState::destructor]      = endFunction;
        closeEventMap[ParserState::destructordecl]  = endFunction;

    }
};

#endif
