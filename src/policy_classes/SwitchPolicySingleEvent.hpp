/**
 * @file SwitchPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_SWITCH_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_SWITCH_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct SwitchData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ConditionData> condition;
    std::shared_ptr<BlockData>     block;

    friend std::ostream& operator<<(std::ostream& out, const SwitchData& switchData) {
        if(!switchData.condition) return out;
        return out << *switchData.condition;
    }
};

class SwitchPolicy : public ConditionalPolicy<SwitchData, srcDispatch::ParserState::switchstmt> {
public:
    SwitchPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : ConditionalPolicy<SwitchData, srcDispatch::ParserState::switchstmt>(listeners) {}
};
#endif
