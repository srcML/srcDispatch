/**
 * @file BlockPolicySingleEvent.hpp
 *
 * MODIFIED FOR STEREOCODE
 *
 */
#ifndef INCLUDED_BLOCK_POLICY_SINGE_EVENT_HPP
#define INCLUDED_BLOCK_POLICY_SINGE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <DeclTypePolicySingleEvent.hpp>
#include <ExprStmtPolicySingleEvent.hpp>
#include <ReturnPolicySingleEvent.hpp>

class ConditionalPolicy;
class SwitchPolicy;
class WhilePolicy;
class ForPolicy;
class DoPolicy;

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

struct BlockData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::vector<std::shared_ptr<DeclData>>       locals;
    std::vector<std::shared_ptr<ExpressionData>>  returns;
    std::vector<std::shared_ptr<ExpressionData>>  expr_stmts;

    std::vector<std::shared_ptr<BlockData>>       blocks;
    std::vector<std::any>                         conditionals;
};


class BlockPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    BlockData     data;
    std::size_t   blockDepth;
    
    DeclTypePolicy   * declstmtPolicy;
    ReturnPolicy     * returnPolicy;
    ExprStmtPolicy   * exprStmtPolicy;
    BlockPolicy      * blockPolicy;
    ConditionalPolicy* conditionalPolicy;
    SwitchPolicy     * switchPolicy;
    WhilePolicy      * whilePolicy;
    ForPolicy        * forPolicy;
    DoPolicy         * doPolicy;

public:
    BlockPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners);

    ~BlockPolicy();

protected:
    std::any DataInner() const override;

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} //doesn't use other parsers

    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;

private:
    void InitializeBlockPolicyHandlers();

    void CollectBlockHandlers();
    void CollectReturnHandlers();
    void CollectExpressionHandlers();
    void CollectDeclstmtHandlers();
    void CollectConditionalHandlers();
    void CollectSwitchHandlers();
    void CollectWhileHandlers();
    void CollectForHandlers();
    void CollectDoHandlers();

};

#endif
