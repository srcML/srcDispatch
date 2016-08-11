#include <srcSAXEventDispatchUtilities.hpp>

#include <exception>

#ifndef INCLUDED_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_TYPE_POLICY_SINGLE_EVENT_HPP

class NamePolicy;
class TypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:
    enum TypeType { NAME, POINTER, REFERENCE, RVALUE, SPECIFIER, NONE };

    struct TypeData {
        std::vector<std::pair<void *, TypeType>> types;

        std::string ToString() const;
        friend std::ostream & operator<<(std::ostream & out, const TypeData & typeData);

    };
    private:
        TypeData data;
        std::size_t typeDepth;

        NamePolicy * namePolicy;

    public:
        TypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners);
        ~TypePolicy();
        virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;
    protected:
        virtual void * DataInner() const override;
    private:
        void InitializeTypePolicyHandlers();

        void CollectNamesHandler();
        void CollectModifersHandler();
        void CollectSpecifiersHandler();

};

#endif
