/**
 * @file WhilePolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_WHILE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_WHILE_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct WhileData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ConditionData> condition;
    std::shared_ptr<BlockData>     block;

    friend std::ostream& operator<<(std::ostream& out, const WhileData& whileData) {
        if(!whileData.condition) return out;
        return out << *whileData.condition;
    }
};

class WhilePolicy : public ConditionalPolicy<WhileData, srcDispatch::ParserState::whilestmt> {
public:
    WhilePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : ConditionalPolicy<WhileData, srcDispatch::ParserState::whilestmt>(listeners) {}
};

#endif
