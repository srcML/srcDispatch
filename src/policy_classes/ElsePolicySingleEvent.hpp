/**
 * @file ElsePolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_ELSE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_ELSE_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <iostream>

struct ElseData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::shared_ptr<ExpressionData> condition;
    std::shared_ptr<BlockData>      block;

    friend std::ostream& operator<<(std::ostream& out, const ElseData& elseData) {
        if(!elseData.condition) return out;
        return out << *elseData.condition;
    }
};

class ElsePolicy : public ConditionalPolicy<ElseData, srcDispatch::ParserState::elsestmt> {
public:
    ElsePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : ConditionalPolicy<ElseData, srcDispatch::ParserState::elsestmt>(listeners) {}
};

#endif
