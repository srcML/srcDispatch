/**
 * @file TemplateArgumentListPolicySingleEvent.hpp
 *
 */
#ifndef INCLUDED_TEMPLATE_ARGUMENT_LIST_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_TEMPLATE_ARGUMENT_LIST_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <NamePolicySingleEvent.hpp>

class NamePolicy;

class ExpressionPolicy;
struct ExpressionData;


struct TemplateArgumentListData {

    unsigned int lineNumber;
    std::vector<std::shared_ptr<ExpressionData>> arguments;

    std::string ToString() const;
    friend std::ostream& operator<<(std::ostream& out, const TemplateArgumentListData& argumentData);
};


class TemplateArgumentListPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

public:
    TemplateArgumentListPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
    ~TemplateArgumentListPolicy();
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

protected:
    virtual std::any DataInner() const override;

private:
    void InitializeTemplateArgumentListPolicyHandlers();
    void CollectArgumentHandler();

private:
    TemplateArgumentListData data;
    std::size_t          argumentListDepth;
    NamePolicy      *    namePolicy;
    ExpressionPolicy*    expressionPolicy;

};

#endif
