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
#include <ExpressionPolicySingleEvent.hpp>
#include <ReturnPolicySingleEvent.hpp>

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
};


class BlockPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    BlockData     data;
    std::size_t   blockDepth;
    
    DeclTypePolicy*   declstmtPolicy;
    ReturnPolicy*     returnPolicy;
    ExpressionPolicy* expressionPolicy;
    BlockPolicy*      blockPolicy;

public:
    BlockPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          blockDepth(0),
          declstmtPolicy(nullptr),
          expressionPolicy(nullptr),
          returnPolicy(nullptr),
          blockPolicy(nullptr) {
        InitializeBlockPolicyHandlers();
    }

    ~BlockPolicy() {
        if (declstmtPolicy)   delete declstmtPolicy;
        if (returnPolicy)     delete returnPolicy;
        if (expressionPolicy) delete expressionPolicy;
        if (blockPolicy)      delete blockPolicy;
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
        } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
            data.expr_stmts.push_back(policy->Data<ExpressionData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }
    }

private:
    void InitializeBlockPolicyHandlers() {
        using namespace srcDispatch;

        CollectBlockHandlers();
        CollectDeclstmtHandlers();
        CollectReturnHandlers();
        CollectExpressionHandlers();

    }

    void CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!blockDepth) {
                blockDepth = ctx.depth;
                data = BlockData{};
            } else {
                if (!blockPolicy) blockPolicy = new BlockPolicy{this};
                ctx.dispatcher->AddListenerDispatch(blockPolicy);
            }
        };

        closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(blockDepth && blockDepth == ctx.depth) {
                blockDepth = 0;
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
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if (!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(expressionPolicy);
        };
    }

    void CollectDeclstmtHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if (!declstmtPolicy) declstmtPolicy = new DeclTypePolicy{this};
            ctx.dispatcher->AddListenerDispatch(declstmtPolicy);
        };
    }

};

#endif
