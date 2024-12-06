/**
 * @file IfPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_IF_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_IF_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct IfData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ConditionData> condition;
    std::shared_ptr<BlockData>     block;

    friend std::ostream& operator<<(std::ostream& out, const IfData& ifData) {
        if(!ifData.condition) return out;
        return out << *ifData.condition;
    }
};

class IfPolicy : public ConditionalPolicy<IfData, srcDispatch::ParserState::ifstmt> {
public:
    IfPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : ConditionalPolicy<IfData, srcDispatch::ParserState::ifstmt>(listeners) {}
};

#endif
