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
#include <ConditionalPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

struct BlockData {

    unsigned int startLineNumber;
    unsigned int endLineNumber;

    std::vector<std::shared_ptr<DeclTypeData>>   locals;
    std::vector<std::shared_ptr<ExpressionData>> returns;
    std::vector<std::shared_ptr<ExpressionData>> expr_stmts;

    std::vector<std::shared_ptr<BlockData>> blocks;
    std::vector<std::shared_ptr<ConditionalData>> conditionals;
};


class BlockPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    BlockData     data;
    std::size_t   blockDepth;
    
    DeclTypePolicy*    declstmtPolicy;
    ReturnPolicy*      returnPolicy;
    ExprStmtPolicy*    exprStmtPolicy;
    BlockPolicy*       blockPolicy;
    ConditionalPolicy* conditionalPolicy;

public:
    BlockPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          blockDepth(0),
          declstmtPolicy(nullptr),
          exprStmtPolicy(nullptr),
          returnPolicy(nullptr),
          blockPolicy(nullptr),
          conditionalPolicy(nullptr) {
        InitializeBlockPolicyHandlers();
    }

    ~BlockPolicy() {
        if (declstmtPolicy)    delete declstmtPolicy;
        if (returnPolicy)      delete returnPolicy;
        if (exprStmtPolicy)    delete exprStmtPolicy;
        if (blockPolicy)       delete blockPolicy;
        if (conditionalPolicy) delete conditionalPolicy;
    }

protected:
    std::any DataInner() const override { return std::make_shared<BlockData>(data); }

    void NotifyWrite(const PolicyDispatcher * policy [[maybe_unused]], srcDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} //doesn't use other parsers

    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override {
        if (typeid(DeclTypePolicy) == typeid(*policy)) {
            std::shared_ptr<std::vector<std::shared_ptr<DeclTypeData>>> decl_data = policy->Data<std::vector<std::shared_ptr<DeclTypeData>>>();
            for(std::shared_ptr<DeclTypeData> decl : *decl_data) {
                data.locals.push_back(decl);
            }
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ReturnPolicy) == typeid(*policy)) {
            data.returns.push_back(policy->Data<ExpressionData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ExprStmtPolicy) == typeid(*policy)) {
            data.expr_stmts.push_back(policy->Data<ExpressionData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(BlockPolicy) == typeid(*policy)) {
            data.blocks.push_back(policy->Data<BlockData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ConditionalPolicy) == typeid(*policy)) {
            data.conditionals.push_back(policy->Data<ConditionalData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
    }

private:
    void InitializeBlockPolicyHandlers() {
        using namespace srcDispatch;

        CollectBlockHandlers();
        CollectDeclstmtHandlers();
        CollectReturnHandlers();
        CollectExpressionHandlers();
        CollectConditionalsHandlers();

    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!blockDepth) {
                blockDepth = ctx.depth;
                data = BlockData{};
                data.startLineNumber = ctx.currentLineNumber;
            } else {
                if (!blockPolicy) blockPolicy = new BlockPolicy{this};
                ctx.dispatcher->AddListenerDispatch(blockPolicy);
            }
        };

        closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(blockDepth && blockDepth == ctx.depth) {
                blockDepth = 0;
                data.endLineNumber = ctx.currentLineNumber;
                NotifyAll(ctx);
                InitializeBlockPolicyHandlers();
            }
        };

    }

    void CollectReturnHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
            if (!returnPolicy) returnPolicy = new ReturnPolicy{this};
            ctx.dispatcher->AddListenerDispatch(returnPolicy);
        };
    }

    void CollectExpressionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
            if (!exprStmtPolicy) exprStmtPolicy = new ExprStmtPolicy{this};
            ctx.dispatcher->AddListenerDispatch(exprStmtPolicy);
        };
    }

    void CollectDeclstmtHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if (!declstmtPolicy) declstmtPolicy = new DeclTypePolicy{this};
            ctx.dispatcher->AddListenerDispatch(declstmtPolicy);
        };
    }

    void CollectConditionalsHandlers() {
        using namespace srcDispatch;
        std::function<void (srcSAXEventContext& ctx)> startConditional = [this](srcSAXEventContext& ctx) {
            if (!conditionalPolicy) conditionalPolicy = new ConditionalPolicy{this};
            ctx.dispatcher->AddListenerDispatch(conditionalPolicy);
        };

        openEventMap[ParserState::ifstmt]     = startConditional;
        openEventMap[ParserState::whilestmt]  = startConditional;
        openEventMap[ParserState::forstmt]    = startConditional;
        openEventMap[ParserState::switchstmt] = startConditional;
        openEventMap[ParserState::dostmt]     = startConditional;


    }

};

#endif
