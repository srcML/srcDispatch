/**
 * @file TypePolicySingleEvent.hpp
 *
  */
#ifndef INCLUDED_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_TYPE_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>

#include <exception>

class NamePolicy;


struct TypeData {
    enum TypeType { TYPENAME, POINTER, REFERENCE, RVALUE, SPECIFIER, NONE };

    unsigned int lineNumber;
    std::vector<std::pair<std::any, TypeType>> types;
    std::string ToString() const;
    friend std::ostream& operator<<(std::ostream& out, const TypeData& typeData);
};


class TypePolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    TypeData data;
    std::size_t typeDepth;
    std::unique_ptr<NamePolicy> namePolicy;

public:
    TypePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners);
    ~TypePolicy();
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

protected:
    virtual std::any DataInner() const override;

private:
    void InitializeTypePolicyHandlers();
    void CollectNamesHandler();
    void CollectModifersHandler();
    void CollectSpecifiersHandler();

};

#endif
