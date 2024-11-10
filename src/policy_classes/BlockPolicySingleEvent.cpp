/**
 * @file BlockPolicySingleEvent.hpp
 *
 * MODIFIED FOR STEREOCODE
 *
 */

#include <BlockPolicySingleEvent.hpp>

#include <srcDispatchUtilities.hpp>

#include <ConditionalPolicySingleEvent.hpp>
#include <WhilePolicySingleEvent.hpp>
#include <ForPolicySingleEvent.hpp>
#include <DoPolicySingleEvent.hpp>

BlockPolicy::BlockPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      blockDepth(0),
      declstmtPolicy(nullptr),
      exprStmtPolicy(nullptr),
      returnPolicy(nullptr),
      blockPolicy(nullptr),
      conditionalPolicy(nullptr),
      whilePolicy(nullptr),
      forPolicy(nullptr),
      doPolicy(nullptr) {
    InitializeBlockPolicyHandlers();
}

BlockPolicy::~BlockPolicy() {
    if (declstmtPolicy)    delete declstmtPolicy;
    if (returnPolicy)      delete returnPolicy;
    if (exprStmtPolicy)    delete exprStmtPolicy;
    if (blockPolicy)       delete blockPolicy;
    if (conditionalPolicy) delete conditionalPolicy;
    if (whilePolicy)       delete whilePolicy;
    if (forPolicy)         delete forPolicy;
    if (doPolicy)          delete forPolicy;
}

std::any BlockPolicy::DataInner() const { return std::make_shared<BlockData>(data); }

void BlockPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    if (typeid(DeclTypePolicy) == typeid(*policy)) {
        std::shared_ptr<std::vector<std::shared_ptr<DeclData>>> decl_data = policy->Data<std::vector<std::shared_ptr<DeclData>>>();
        for(std::shared_ptr<DeclData> decl : *decl_data) {
            data.locals.push_back(decl);
        }
    } else if (typeid(ReturnPolicy) == typeid(*policy)) {
        data.returns.push_back(policy->Data<ExpressionData>());
    } else if (typeid(ExprStmtPolicy) == typeid(*policy)) {
        data.expr_stmts.push_back(policy->Data<ExpressionData>());
    } else if (typeid(BlockPolicy) == typeid(*policy)) {
        data.blocks.push_back(policy->Data<BlockData>());
    } else if (typeid(ConditionalPolicy) == typeid(*policy)) {
        data.conditionals.push_back(policy->Data<ConditionalData>());
    } else if (typeid(WhilePolicy) == typeid(*policy)) {
        data.conditionals.push_back(policy->Data<WhileData>());
    } else if (typeid(ForPolicy) == typeid(*policy)) {
        data.conditionals.push_back(policy->Data<ForData>());
    } else if (typeid(DoPolicy) == typeid(*policy)) {
        data.conditionals.push_back(policy->Data<DoData>());
    }  else {
        throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
    }

    ctx.dispatcher->RemoveListenerDispatch(nullptr);
}

void BlockPolicy::InitializeBlockPolicyHandlers() {
    using namespace srcDispatch;

    CollectBlockHandlers();
    CollectDeclstmtHandlers();
    CollectReturnHandlers();
    CollectExpressionHandlers();
    CollectConditionalHandlers();
    CollectWhileHandlers();
    CollectDoHandlers();

}

void BlockPolicy::CollectBlockHandlers() {
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

void BlockPolicy::CollectReturnHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
        if (!returnPolicy) returnPolicy = new ReturnPolicy{this};
        ctx.dispatcher->AddListenerDispatch(returnPolicy);
    };
}

void BlockPolicy::CollectExpressionHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
        if (!exprStmtPolicy) exprStmtPolicy = new ExprStmtPolicy{this};
        ctx.dispatcher->AddListenerDispatch(exprStmtPolicy);
    };
}

void BlockPolicy::CollectDeclstmtHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
        if (!declstmtPolicy) declstmtPolicy = new DeclTypePolicy{this};
        ctx.dispatcher->AddListenerDispatch(declstmtPolicy);
    };
}

void BlockPolicy::CollectConditionalHandlers() {
    using namespace srcDispatch;
    std::function<void (srcSAXEventContext& ctx)> startConditional = [this](srcSAXEventContext& ctx) {
        if (!conditionalPolicy) conditionalPolicy = new ConditionalPolicy{this};
        ctx.dispatcher->AddListenerDispatch(conditionalPolicy);
    };

    openEventMap[ParserState::ifstmt]     = startConditional;
    openEventMap[ParserState::switchstmt] = startConditional;
    openEventMap[ParserState::dostmt]     = startConditional;

}

void BlockPolicy::CollectWhileHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
        if (!whilePolicy) whilePolicy = new WhilePolicy{this};
        ctx.dispatcher->AddListenerDispatch(whilePolicy);
    };

}

void BlockPolicy::CollectForHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
        if (!forPolicy) forPolicy = new ForPolicy{this};
        ctx.dispatcher->AddListenerDispatch(forPolicy);
    };

}

void BlockPolicy::CollectDoHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
        if (!forPolicy) forPolicy = new ForPolicy{this};
        ctx.dispatcher->AddListenerDispatch(forPolicy);
    };

}
