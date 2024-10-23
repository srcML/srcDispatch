/**
 * @file ForPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_FOR_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_FOR_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <ControlPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

class BlockPolicy;
class BlockData;

struct ForData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ControlData> control;
    std::shared_ptr<BlockData>   block;

    friend std::ostream& operator<<(std::ostream& out, const ForData& conditionalData);
};


class ForPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ForData  data;
    std::size_t      conditionalDepth;
    ControlPolicy  * controlPolicy;
    BlockPolicy    * blockPolicy;

public:
    ForPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners);
    ~ForPolicy();

protected:
    std::any DataInner() const override;
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) override;

private:
    void InitializeForPolicyHandlers();
    void CollectControlHandlers();
    void CollectBlockHandlers();
};

#endif
