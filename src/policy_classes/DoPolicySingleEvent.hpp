/**
 * @file DoPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_DO_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DO_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct DoData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ConditionData> condition;
    std::shared_ptr<BlockData>     block;

    friend std::ostream& operator<<(std::ostream& out, const DoData& doData) {
        if(!doData.condition) return out;
        return out << *doData.condition;
    }
};

class DoPolicy : public ConditionalPolicy<DoData, srcDispatch::ParserState::dostmt> {
public:
    DoPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : ConditionalPolicy<DoData, srcDispatch::ParserState::dostmt>(listeners) {}
};

#endif
