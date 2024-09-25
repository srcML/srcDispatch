/**
 * @file CallPolicySingleEvent.hpp
 *
 */
#ifndef INCLUDED_CALL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CALL_POLICY_SINGLE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
#include <TemplateArgumentPolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

//
// Collects <call> information
//  foo(x)
//  obj.foo(x)
//
//  Gets the name and list of arguments
//

struct ExpressionData;
class ExpressionPolicy;

struct NameData;
class NamePolicy;

struct CallData {
    std::shared_ptr<NameData> name;
    std::vector<std::shared_ptr<ExpressionData>> arguments;  //expressions

    friend std::ostream & operator<<(std::ostream & out, const CallData &call);
};

class CallPolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    CallData            data;
    std::size_t         callDepth;
    NamePolicy          *namePolicy;
    ExpressionPolicy    *expressionPolicy;

public:
    CallPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          callDepth(0),
          expressionPolicy(nullptr),
          namePolicy(nullptr) {
        InitializeCallPolicyHandlers();
    }

    ~CallPolicy();

protected:
    std::any DataInner() const override;
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;
    void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

private:
    void InitializeCallPolicyHandlers();
    void CollectNameHandlers();
    void CollectCallArgumentHandlers();
};

#endif
