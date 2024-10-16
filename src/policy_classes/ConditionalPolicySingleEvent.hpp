/**
 * @file ConditionalPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_CONDITIONAL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CONDITIONAL_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ConditionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

class BlockPolicy;
class BlockData;

struct ConditionalData {

    enum ConditionalType { IF, WHILE, FOR, FOREACH, SWITCH, DO };

    ConditionalType type;

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;
};


class ConditionalPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ConditionalData  data;
    std::size_t      conditionalDepth;
    ConditionPolicy* conditionPolicy;
    BlockPolicy    * blockPolicy;

public:
    ConditionalPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
    ~ConditionalPolicy();

protected:
    std::any DataInner() const override;
    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override;
    void NotifyWrite(const PolicyDispatcher * policy, srcDispatch::srcSAXEventContext & ctx) override;

private:
    void InitializeConditionalPolicyHandlers();
    void CollectConditionHandlers();
    void CollectBlockHandlers();
};

#endif
