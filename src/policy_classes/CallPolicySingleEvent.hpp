/**
 * @file CallPolicySingleEvent.hpp
 *
 */
#ifndef INCLUDED_CALL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CALL_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

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

    unsigned int lineNumber;
    std::shared_ptr<NameData> name;
    std::vector<std::shared_ptr<ExpressionData>> arguments;  //expressions

    friend std::ostream & operator<<(std::ostream & out, const CallData &call);
};

class CallPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    CallData            data;
    std::size_t         callDepth;
    NamePolicy          *namePolicy;
    ExpressionPolicy    *expressionPolicy;

public:
    CallPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
          data{},
          callDepth(0),
          expressionPolicy(nullptr),
          namePolicy(nullptr) {
        InitializeCallPolicyHandlers();
    }

    ~CallPolicy();

protected:
    std::any DataInner() const override;
    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override;
    void NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

private:
    void InitializeCallPolicyHandlers();
    void CollectNameHandlers();
    void CollectCallArgumentHandlers();
};

#endif
