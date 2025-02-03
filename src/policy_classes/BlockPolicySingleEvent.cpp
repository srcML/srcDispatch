/**
 * @file BlockPolicySingleEvent.hpp
 *
 * MODIFIED FOR STEREOCODE
 *
 */

#include <BlockPolicySingleEvent.hpp>

#include <srcDispatchUtilities.hpp>

#include <IfStmtPolicySingleEvent.hpp>
#include <SwitchPolicySingleEvent.hpp>
#include <WhilePolicySingleEvent.hpp>
#include <ForPolicySingleEvent.hpp>
#include <DoPolicySingleEvent.hpp>

BlockPolicy::BlockPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
    : srcDispatch::PolicyDispatcher(listeners),
      data{},
      blockDepth(0) {
    InitializeBlockPolicyHandlers();
}

BlockPolicy::~BlockPolicy() {}

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
    } else if (typeid(IfStmtPolicy) == typeid(*policy)) {
        data.conditionals.push_back(policy->Data<IfStmtData>());
    } else if (typeid(SwitchPolicy) == typeid(*policy)) {
        data.conditionals.push_back(policy->Data<SwitchData>());
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
    CollectIfStmtHandlers();
    CollectSwitchHandlers();
    CollectWhileHandlers();
    CollectForHandlers();
    CollectDoHandlers();

}

void BlockPolicy::CollectBlockHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
        if(!blockDepth) {
            blockDepth = ctx.depth;
            data = BlockData{};
            data.startLineNumber = ctx.startLineNumber;
        } else {
            if (!blockPolicy) blockPolicy = make_unique_policy<BlockPolicy>({this});
            ctx.dispatcher->AddListenerDispatch(blockPolicy.get());
        }
    };

    closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
        if(blockDepth && blockDepth == ctx.depth) {
            blockDepth = 0;
            data.endLineNumber = ctx.startLineNumber;
            NotifyAll(ctx);
            InitializeBlockPolicyHandlers();
        }
    };

}

void BlockPolicy::CollectReturnHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
        if (!returnPolicy) returnPolicy = make_unique_policy<ReturnPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(returnPolicy.get());
    };
}

void BlockPolicy::CollectExpressionHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
        if (!exprStmtPolicy) exprStmtPolicy = make_unique_policy<ExprStmtPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(exprStmtPolicy.get());
    };
}

void BlockPolicy::CollectDeclstmtHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
        if (!declstmtPolicy) declstmtPolicy = make_unique_policy<DeclTypePolicy>({this});
        ctx.dispatcher->AddListenerDispatch(declstmtPolicy.get());
    };
}

void BlockPolicy::CollectIfStmtHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::ifgroup] = [this](srcSAXEventContext& ctx) {
        if (!ifStmtPolicy) ifStmtPolicy = make_unique_policy<IfStmtPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(ifStmtPolicy.get());
    };

}

void BlockPolicy::CollectSwitchHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::switchstmt] = [this](srcSAXEventContext& ctx) {
        if (!switchPolicy) switchPolicy = make_unique_policy<SwitchPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(switchPolicy.get());
    };

}

void BlockPolicy::CollectWhileHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
        if (!whilePolicy) whilePolicy = make_unique_policy<WhilePolicy>({this});
        ctx.dispatcher->AddListenerDispatch(whilePolicy.get());
    };

}

void BlockPolicy::CollectForHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
        if (!forPolicy) forPolicy = make_unique_policy<ForPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(forPolicy.get());
    };

}

void BlockPolicy::CollectDoHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::dostmt] = [this](srcSAXEventContext& ctx) {
        if (!doPolicy) doPolicy = make_unique_policy<DoPolicy>({this});
        ctx.dispatcher->AddListenerDispatch(doPolicy.get());
    };

}
