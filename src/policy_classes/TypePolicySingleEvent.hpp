/**
 * @file TypePolicySingleEvent.hpp
 *
  */
#ifndef INCLUDED_TYPE1_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_TYPE1_POLICY_SINGLE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>

#include <exception>

class NamePolicy;


struct TypeData {
    enum TypeType { TYPENAME, POINTER, REFERENCE, RVALUE, SPECIFIER, NONE };

    std::vector<std::pair<std::any, TypeType>> types;
    std::string ToString() const;
    friend std::ostream & operator<<(std::ostream & out, const TypeData & typeData);
};


class TypePolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    TypeData data;
    std::size_t typeDepth;
    NamePolicy * namePolicy;

public:
    TypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners);
    ~TypePolicy();
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;
    virtual void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override;

protected:
    virtual std::any DataInner() const override;

private:
    void InitializeTypePolicyHandlers();
    void CollectNamesHandler();
    void CollectModifersHandler();
    void CollectSpecifiersHandler();

};

#endif
