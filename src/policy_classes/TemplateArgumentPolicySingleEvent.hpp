/**
 * @file TemplateArgumentPolicySingleEvent.hpp
 *
 */
#ifndef INCLUDED_TEMPLATE1_ARGUMENT_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_TEMPLATE1_ARGUMENT_POLICY_SINGLE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>
#include <NamePolicySingleEvent.hpp>

class NamePolicy;


struct TemplateArgumentData {
    enum TemplateArgumentType { NAME, LITERAL, MODIFIER, POINTER, REFERENCE, RVALUE, OPERATOR, CALL };

    unsigned int lineNumber;
    std::vector<std::pair<std::any, TemplateArgumentType>> data;
    friend std::ostream& operator<<(std::ostream& out, const TemplateArgumentData& argumentData);
};


class TemplateArgumentPolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

public:
    TemplateArgumentPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners);
    ~TemplateArgumentPolicy();
    virtual void Notify(const PolicyDispatcher* policy, const srcSAXEventDispatch::srcSAXEventContext& ctx) override;
    virtual void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcSAXEventDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override;

protected:
    virtual std::any DataInner() const override;

private:
    void InitializeTemplateArgumentPolicyHandlers();
    void CollectNamesHandler();
    void CollectOthersHandler();

private:
    TemplateArgumentData data;
    std::size_t argumentDepth;
    NamePolicy* namePolicy;

};

#endif
