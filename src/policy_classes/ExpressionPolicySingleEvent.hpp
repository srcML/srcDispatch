/**
 * @file ExpressionPolicy.hpp
 *
 */
#ifndef INCLUDED_EXPRESSION_POLICY_SINGE_EVENT_HPP
#define INCLUDED_EXPRESSION_POLICY_SINGE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
#include <OperatorPolicySingleEvent.hpp>
#include <LiteralPolicySingleEvent.hpp>
#include <CallPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

struct CallData;
class CallPolicy;

struct NameData;
class NamePolicy;

//A vector of elements in the expression.
//Names, operators, calls in the correct order.
//Need for determining variable use, variable modification, calls

struct ExpressionData {

    unsigned int lineNumber;
    std::vector<std::any> expr;

    friend std::ostream& operator<<(std::ostream& out, const ExpressionData& ex);
};

class ExpressionPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    ExpressionData  data;
    std::size_t     exprDepth;
    std::unique_ptr<NamePolicy>     namePolicy;
    std::unique_ptr<OperatorPolicy> operatorPolicy;
    std::unique_ptr<LiteralPolicy>  literalPolicy;
    std::unique_ptr<CallPolicy>     callPolicy;

public:
    ExpressionPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
    data{},
    exprDepth(0) {
        InitializeExpressionPolicyHandlers();
    }

    ~ExpressionPolicy();

protected:
    std::any DataInner() const override { return std::make_shared<ExpressionData>(data); }
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeExpressionPolicyHandlers();
    void CollectCallHandlers();
    void CollectNameHandlers();
    void CollectOperatorHandlers();
    void CollectLiteralHandlers();
};

#endif
