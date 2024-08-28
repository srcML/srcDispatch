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

    std::vector<std::pair<void *, TemplateArgumentType>> data;
    friend std::ostream & operator<<(std::ostream & out, const TemplateArgumentData & argumentData);
};


class TemplateArgumentPolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    TemplateArgumentData data;
    std::size_t argumentDepth;
    NamePolicy * namePolicy;

public:
    TemplateArgumentPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners);
    ~TemplateArgumentPolicy();
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;
    virtual void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override;

protected:
    virtual void * DataInner() const override;

private:
    void InitializeTemplateArgumentPolicyHandlers();
    void CollectNamesHandler();
    void CollectOthersHandler();

};

#endif
