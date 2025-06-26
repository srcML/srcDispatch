// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file IfStmtPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www..org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_IF_STMT_POLICY_HPP
#define INCLUDED_SRCDIFF_IF_STMT_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <IfPolicy.hpp>
#include <ElseIfPolicy.hpp>
#include <ElsePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct IfStmtData {

        unsigned int startLineNumber;
        unsigned int endLineNumber;

        std::vector<DeltaElement<std::any>> clauses;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            if(clauses.empty()) return "";

            // Not sure how safe using GetElement is here
            if(clauses.front().GetElement().type() != typeid(std::shared_ptr<IfData>)) return "";

            return clauses.front().ToString<std::shared_ptr<IfData>>(operation);
        }
    };

    class IfStmtPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        IfStmtData  data;

        std::unique_ptr<IfPolicy>     ifPolicy;
        std::unique_ptr<ElseIfPolicy> elseIfPolicy;
        std::unique_ptr<ElsePolicy>   elsePolicy;

    public:
        IfStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeIfStmtPolicyHandlers();
        }

        ~IfStmtPolicy() {}

    protected:
        std::any DataInner() const { return std::make_shared<IfStmtData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
            if(typeid(IfPolicy) == typeid(*policy)) {
                data.clauses.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<IfData>()));
            } else if(typeid(ElseIfPolicy) == typeid(*policy)) {
                data.clauses.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<ElseIfData>()));
            } else if(typeid(ElsePolicy) == typeid(*policy)) {
                data.clauses.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<ElseData>()));
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) {} // doesn't use other parsers

    private:
        void InitializeIfStmtPolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::ifgroup] = [this](srcSAXEventContext& ctx) {
                if(!depth) {
                    depth = ctx.depth;
                    data = IfStmtData{};
                    data.startLineNumber = ctx.startLineNumber;
                    data.endLineNumber = ctx.endLineNumber;
                    CollectIfHandlers();
                    CollectElseIfHandlers();
                    CollectElseHandlers();
                }
            };

            // end of policy
            closeEventMap[ParserState::ifgroup] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeIfStmtPolicyHandlers();
            };
        }

        void CollectIfHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::ifstmt] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!ifPolicy) {
                    ifPolicy = make_unique_policy<IfPolicy>({this});
                }

                ctx.dispatcher->AddListenerDispatch(ifPolicy.get());
            };
        }

        void CollectElseIfHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::elseif] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!elseIfPolicy) {
                    elseIfPolicy = make_unique_policy<ElseIfPolicy>({this});
                }

                ctx.dispatcher->AddListenerDispatch(elseIfPolicy.get());
            };
        }

        void CollectElseHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::elsestmt] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!elsePolicy) {
                    elsePolicy = make_unique_policy<ElsePolicy>({this});
                }

                ctx.dispatcher->AddListenerDispatch(elsePolicy.get());
            };
        }
    };

}

#endif
