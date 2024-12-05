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

class IfStmtPolicy;
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

    std::vector<std::shared_ptr<DeclData>>        locals;
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
    
    std::unique_ptr<DeclTypePolicy> declstmtPolicy;
    std::unique_ptr<ReturnPolicy>   returnPolicy;
    std::unique_ptr<ExprStmtPolicy> exprStmtPolicy;
    std::unique_ptr<BlockPolicy>    blockPolicy;
    std::unique_ptr<IfStmtPolicy>   ifStmtPolicy;
    std::unique_ptr<SwitchPolicy>   switchPolicy;
    std::unique_ptr<WhilePolicy>    whilePolicy;
    std::unique_ptr<ForPolicy>      forPolicy;
    std::unique_ptr<DoPolicy>       doPolicy;

public:
    BlockPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners);

    ~BlockPolicy();

protected:
    std::any DataInner() const override;
    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override;
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeBlockPolicyHandlers();

    void CollectBlockHandlers();
    void CollectReturnHandlers();
    void CollectExpressionHandlers();
    void CollectDeclstmtHandlers();
    void CollectIfStmtHandlers();
    void CollectSwitchHandlers();
    void CollectWhileHandlers();
    void CollectForHandlers();
    void CollectDoHandlers();

};

#endif
