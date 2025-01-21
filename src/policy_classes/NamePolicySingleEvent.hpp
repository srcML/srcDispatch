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

#include <TemplateArgumentListPolicySingleEvent.hpp>
#include <OperatorPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

class  ExpressionPolicy;
struct ExpressionData;

class TemplateArgumentListPolicy;
struct TemplateArgumentListData;

struct NameData {

    unsigned int lineNumber;
    std::string                                  name;
    std::vector<std::any>                        names;
    std::shared_ptr<TemplateArgumentListData>    templateArgumentList;
    std::vector<std::shared_ptr<ExpressionData>> indices;

    std::string SimpleName() const;
    std::string ToString() const;
    friend std::ostream& operator<<(std::ostream& out, const NameData& nameData);
};
 

class NamePolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    NameData                                    data;
    std::size_t                                 nameDepth;
    std::unique_ptr<NamePolicy>                 namePolicy;
    std::unique_ptr<OperatorPolicy>             operatorPolicy;
    std::unique_ptr<TemplateArgumentListPolicy> templateArgumentListPolicy;
    std::unique_ptr<ExpressionPolicy>           expressionPolicy;

public:
    NamePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          nameDepth(0) {
        InitializeNamePolicyHandlers();
    }

    ~NamePolicy();

protected:

    std::any DataInner() const override { return std::make_shared<NameData>(data); }
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeNamePolicyHandlers();
    void CollectOperatorsHandlers();
    void CollectTemplateArgumentListHandlers();
    void CollectArrayIndicesHandlers();
};

#endif
