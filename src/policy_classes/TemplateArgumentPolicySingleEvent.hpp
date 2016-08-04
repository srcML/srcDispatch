#include <srcSAXEventDispatchUtilities.hpp>

#include <exception>

#ifndef INCLUDED_TEMPLATE_ARGUMENT_POLICY_HPP
#define INCLUDED_TEMPLATE_ARGUMENT_POLICY_HPP

class NamePolicy;
class TemplateArgumentPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:
    enum TemplateArgumentType { NAME, LITERAL, OTHER };

    struct TemplateArgumentData {
        std::vector<std::pair<void *, TemplateArgumentType>> data;

        friend std::ostream & operator<<(std::ostream & out, const TemplateArgumentData & argumentData);

    };
    private:
        TemplateArgumentData data;
        std::size_t argumentDepth;
        NamePolicy * namePolicy;

    public:
        TemplateArgumentPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners);
        ~TemplateArgumentPolicy();
        virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;
    protected:
        virtual void * DataInner() const override;
    private:
        void InitializeTemplateArgumentPolicyHandlers();

        void CollectNamesHandler();
        void CollectOthersHandler();

};

#endif
