/**
 * @file ExpressionPolicy.hpp
 *
 */
#ifndef INCLUDED_EXPRESSION_POLICY_SINGE_EVENT_HPP
#define INCLUDED_EXPRESSION_POLICY_SINGE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
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

struct Token {
    Token(unsigned int lineNumber, const std::string& token) 
        : lineNumber(lineNumber), token(token) {
    }
    unsigned int lineNumber;
    std::string token;
};

struct ExpressionElement {
    enum ExprType { NAME, OP, CALL, LITERAL};

    ExprType type;
    std::shared_ptr<NameData> name;
    std::shared_ptr<Token>    token;
    std::shared_ptr<CallData> call;
    ExpressionElement(ExprType t, std::shared_ptr<NameData> item) : type(t), name(item) {};
    ExpressionElement(ExprType t, std::shared_ptr<Token>    item) : type(t), token(item){};
    ExpressionElement(ExprType t, std::shared_ptr<CallData> item) : type(t), call(item) {};
};

struct ExpressionData {

    unsigned int lineNumber;
    std::vector<std::shared_ptr<ExpressionElement>> expr;   //All items in expression

    friend std::ostream & operator<<(std::ostream & out, const ExpressionData & ex);
};

class ExpressionPolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    ExpressionData  data;
    NamePolicy      *namePolicy;
    CallPolicy      *callPolicy;
    std::size_t     exprDepth;

public:
    ExpressionPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
    data{},
    exprDepth(0),
    namePolicy(nullptr),
    callPolicy(nullptr)
    {
        InitializeExpressionPolicyHandlers();
    }

    ~ExpressionPolicy();

protected:
    std::any DataInner() const override { return std::make_shared<ExpressionData>(data); }

    void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers

    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override;

private:
    void InitializeExpressionPolicyHandlers();
    void CollectCallHandlers();
    void CollectNameHandlers();
    void CollectOtherHandlers();
};

#endif
