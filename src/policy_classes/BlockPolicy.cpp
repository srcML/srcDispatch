// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file BlockPolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#include <BlockPolicy.hpp>

#include <srcDispatchUtilities.hpp>

#include <DeltaElement.hpp>
#include <ConvertPlexerPolicy.hpp>

#include <IfStmtPolicy.hpp>
#include <SwitchPolicy.hpp>
#include <WhilePolicy.hpp>
#include <ForPolicy.hpp>
#include <DoPolicy.hpp>
#include <TryPolicy.hpp>
#include <ClassPolicy.hpp>

namespace srcDispatch {

    BlockPolicy::BlockPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners), data{} {
        InitializeBlockPolicyHandlers();
    }

    BlockPolicy::~BlockPolicy() {}

    std::any BlockPolicy::DataInner() const { return std::make_shared<BlockData>(data); }

    void BlockPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        if(typeid(DeclStmtPolicy) == typeid(*policy)) {
            data.statements.emplace_back(ctx.diffStack.back().operation, policy->Data<DeclStmtData>());
        } else if(typeid(ExprStmtPolicy) == typeid(*policy)) {
            data.statements.emplace_back(ctx.diffStack.back().operation, policy->Data<ExprStmtData>());
        } else if(typeid(ReturnPolicy) == typeid(*policy)) {
            data.statements.emplace_back(ctx.diffStack.back().operation, policy->Data<ReturnData>());
        } else if(typeid(IfStmtPolicy) == typeid(*policy)) {
            data.statements.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<IfStmtData>()));
        } else if(typeid(SwitchPolicy) == typeid(*policy)) {
            data.statements.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<SwitchData>()));
        } else if(typeid(WhilePolicy) == typeid(*policy)) {
            data.statements.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<WhileData>()));
        } else if(typeid(ForPolicy) == typeid(*policy)) {
            data.statements.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<ForData>()));
        } else if(typeid(DoPolicy) == typeid(*policy)) {
            data.statements.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<DoData>()));
        } else if(typeid(TryPolicy) == typeid(*policy)) {
            data.statements.emplace_back(ctx.diffStack.back().operation, policy->Data<TryData>());
        } else if(typeid(ThrowPolicy) == typeid(*policy)) {
            data.statements.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<ThrowData>()));
        } else if(typeid(GotoPolicy) == typeid(*policy)) {
            data.statements.emplace_back(ctx.diffStack.back().operation, policy->Data<GotoData>());
        } else if(typeid(ConvertPlexerPolicy) == typeid(*policy)) {
            data.statements.push_back(policy->Data<ConvertData>()->construct);
        } else if(typeid(ClassPolicy) == typeid(*policy)) {
            srcDispatch::DiffOperation operation = ctx.diffStack.back().isConvert? srcDispatch::COMMON : ctx.diffStack.back().operation;
            data.localClasses.emplace_back(operation, policy->Data<ClassData>());
        }  else if(typeid(BlockPolicy) == typeid(*policy)) {
            data.blocks.emplace_back(ctx.diffStack.back().operation, policy->Data<BlockData>());
        } else if(typeid(CasePolicy) == typeid(*policy)) {
            data.cases.emplace_back(ctx.diffStack.back().operation, policy->Data<CaseData>());
        } else if(typeid(LabelPolicy) == typeid(*policy)) {
            data.labels.emplace_back(ctx.diffStack.back().operation, policy->Data<LabelData>());
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    }

    void BlockPolicy::InitializeBlockPolicyHandlers() {
        using namespace srcDispatch;

        CollectBlockHandlers();
        CollectDeclstmtHandlers();
        CollectExpressionHandlers();
        CollectReturnHandlers();
        CollectIfStmtHandlers();
        CollectSwitchHandlers();
        CollectWhileHandlers();
        CollectForHandlers();
        CollectDoHandlers();
        CollectTryHandlers();
        CollectThrowHandlers();
        CollectGotoHandlers();
        CollectClassHandlers();
        CollectCaseHandlers();
        CollectLabelHandlers();
    }

    template<typename type>
    bool BlockPolicy::ConvertRegistrationCheck(srcDispatch::srcSAXEventContext& ctx) {
        if(!ctx.diffStack.back().isConvert) return false;

        if(!plexer) {
            plexer = srcDispatch::make_unique_policy<ConvertPlexerPolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(plexer.get());

        return true;
    }

    void BlockPolicy::CollectBlockHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!depth) {
                depth = ctx.depth;
                data = BlockData{};
                data.startLineNumber = ctx.startLineNumber;
                data.endLineNumber   = ctx.endLineNumber;
            } else {
                if(!blockPolicy)
                    blockPolicy = make_unique_policy<BlockPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(blockPolicy.get());
            }
        };

        closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeBlockPolicyHandlers();
        };
    }

    void BlockPolicy::CollectDeclstmtHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            if(ConvertRegistrationCheck<DeclStmtPolicy>(ctx)) return;

            if(!declstmtPolicy) {
                declstmtPolicy = make_unique_policy<DeclStmtPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(declstmtPolicy.get());
        };

        closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(plexer) {
                plexer.reset();
            }
        };
    }

    void BlockPolicy::CollectExpressionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            if(ConvertRegistrationCheck<ExprStmtPolicy>(ctx)) return;

            if(!exprStmtPolicy) {
                exprStmtPolicy = make_unique_policy<ExprStmtPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(exprStmtPolicy.get());
        };

        closeEventMap[ParserState::exprstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(plexer) {
                plexer.reset();
            }
        };
    }

    void BlockPolicy::CollectReturnHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            if(ConvertRegistrationCheck<ReturnPolicy>(ctx)) return;

            if(!returnPolicy) {
                returnPolicy = make_unique_policy<ReturnPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(returnPolicy.get());
        };

        closeEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(plexer) {
                plexer.reset();
            }
        };
    }

    void BlockPolicy::CollectIfStmtHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::ifgroup] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            if(ConvertRegistrationCheck<IfStmtPolicy>(ctx)) return;

            if(!ifStmtPolicy) {
                ifStmtPolicy = make_unique_policy<IfStmtPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(ifStmtPolicy.get());
        };

        closeEventMap[ParserState::ifgroup] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(plexer) {
                plexer.reset();
            }
        };
    }

    void BlockPolicy::CollectSwitchHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::switchstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!switchPolicy) {
                switchPolicy = make_unique_policy<SwitchPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(switchPolicy.get());
        };
    }

    void BlockPolicy::CollectWhileHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            if(ConvertRegistrationCheck<WhilePolicy>(ctx)) return;

            if(!whilePolicy) {
                whilePolicy = make_unique_policy<WhilePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(whilePolicy.get());
        };

        closeEventMap[ParserState::whilestmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(plexer) {
                plexer.reset();
            }
        };
    }

    void BlockPolicy::CollectForHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;
            if(ConvertRegistrationCheck<ForPolicy>(ctx)) return;

            if(!forPolicy) {
                forPolicy = make_unique_policy<ForPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(forPolicy.get());
        };

        closeEventMap[ParserState::forstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(plexer) {
                plexer.reset();
            }
        };
    }

    void BlockPolicy::CollectDoHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::dostmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!doPolicy) {
                doPolicy = make_unique_policy<DoPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(doPolicy.get());
        };
    }

    void BlockPolicy::CollectTryHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::trystmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!tryPolicy) {
                tryPolicy = make_unique_policy<TryPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(tryPolicy.get());
        };
    }

    void BlockPolicy::CollectThrowHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::throwstmt] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!throwPolicy) {
                throwPolicy = make_unique_policy<ThrowPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(throwPolicy.get());
        };
    }

    void BlockPolicy::CollectGotoHandlers() {
        using namespace srcDispatch;
         std::function<void(srcSAXEventContext& ctx)> startGoto = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!gotoPolicy) {
                gotoPolicy = make_unique_policy<GotoPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(gotoPolicy.get());
        };

        openEventMap[ParserState::gotostmt]     = startGoto;
        openEventMap[ParserState::breakstmt]    = startGoto;
        openEventMap[ParserState::continuestmt] = startGoto;
    }

    void BlockPolicy::CollectClassHandlers() {
        using namespace srcDispatch;
        std::function<void(srcDispatch::srcSAXEventContext& )> startClassPolicy = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!classPolicy) {
                classPolicy = make_unique_policy<ClassPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(classPolicy.get());
        };

        openEventMap[ParserState::classn]  = startClassPolicy;
        openEventMap[ParserState::structn] = startClassPolicy;
    }

    void BlockPolicy::CollectCaseHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::switchcase] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!casePolicy) {
                casePolicy = make_unique_policy<CasePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(casePolicy.get());
        };
    }

    void BlockPolicy::CollectLabelHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::label] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!labelPolicy) {
                labelPolicy = make_unique_policy<LabelPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(labelPolicy.get());
        };
    }

}
