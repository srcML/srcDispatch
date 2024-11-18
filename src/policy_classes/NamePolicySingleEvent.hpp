/**
 * @file NamePolicySingleEvent.hpp
 *
 * MODIFIED from srcDispatch
 *  This collects the expression in the index
 *
  */
#ifndef INCLUDED_NAME_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_NAME_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <TemplateArgumentPolicySingleEvent.hpp>
#include <OperatorPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ExpressionData;
class  ExpressionPolicy;

struct TemplateArgumentData;
class  TemplateArgumentPolicy;

struct NameData {

    unsigned int lineNumber;
    std::string                                        name;
    std::vector<std::any>                              names;
    std::vector<std::shared_ptr<TemplateArgumentData>> templateArguments;
    std::shared_ptr<ExpressionData>                    indices;

    std::string SimpleName() const;
    std::string ToString() const;
    friend std::ostream& operator<<(std::ostream& out, const NameData& nameData);
};
 

class NamePolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    NameData                data;
    std::size_t             nameDepth;
    NamePolicy            * namePolicy;
    OperatorPolicy        * operatorPolicy;
    TemplateArgumentPolicy* templateArgumentPolicy;
    ExpressionPolicy      * expressionPolicy;

public:
    NamePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          nameDepth(0),
          namePolicy(nullptr),
          operatorPolicy(nullptr),
          templateArgumentPolicy(nullptr),
          expressionPolicy(nullptr) {
        InitializeNamePolicyHandlers();
    }

    ~NamePolicy();

protected:

    std::any DataInner() const override { return std::make_shared<NameData>(data); }
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} //doesn't use other parsers

private:
    void InitializeNamePolicyHandlers();
    void CollectOperatorsHandlers();
    void CollectTemplateArgumentsHandlers();
    void CollectArrayIndicesHandlers();
};

#endif
