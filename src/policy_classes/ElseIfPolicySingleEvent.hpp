/**
 * @file ElseIfPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_ELSEIF_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_ELSEIF_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ElseIfData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ConditionData> condition;
    std::shared_ptr<BlockData>     block;

    friend std::ostream& operator<<(std::ostream& out, const ElseIfData& elseIfData) {
        if(!elseIfData.condition) return out;
        return out << *elseIfData.condition;
    }
};

class ElseIfPolicy : public ConditionalPolicy<ElseIfData, srcDispatch::ParserState::elseif> {
public:
    ElseIfPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : ConditionalPolicy<ElseIfData, srcDispatch::ParserState::elseif>(listeners) {}
};

#endif
