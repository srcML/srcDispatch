/**
 * @file NamePolicySingleEvent.hpp
 *
 * MODIFIED from srcSAXEventDispatcher
 *  This collects the expression in the index
 *
  */
#ifndef INCLUDED_NAME_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_NAME_POLICY_SINGLE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <TemplateArgumentPolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>

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
    std::vector<std::shared_ptr<NameData>>             names;
    std::vector<std::shared_ptr<TemplateArgumentData>> templateArguments;
    std::shared_ptr<ExpressionData>                    indices;

    std::string SimpleName() const;
    std::string ToString() const;
    friend std::ostream& operator<<(std::ostream& out, const NameData& nameData);
};
 

class NamePolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    NameData                    data;
    std::size_t                 nameDepth;
    NamePolicy*                 namePolicy;
    TemplateArgumentPolicy*     templateArgumentPolicy;
    ExpressionPolicy*           expressionPolicy;

public:
    NamePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          nameDepth(0),
          namePolicy(nullptr),
          expressionPolicy(nullptr),
          templateArgumentPolicy(nullptr) {
        InitializeNamePolicyHandlers();
    }

    ~NamePolicy();

protected:

    std::any DataInner() const override { return std::make_shared<NameData>(data); }
    virtual void Notify(const PolicyDispatcher* policy, const srcSAXEventDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcSAXEventDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} //doesn't use other parsers

private:
    void InitializeNamePolicyHandlers();
    void CollectTemplateArgumentsHandlers();
    void CollectArrayIndicesHandlers();
};

#endif
