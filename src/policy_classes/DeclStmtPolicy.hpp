// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DeclStmtPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_DECL_TYPE_POLICY_HPP
#define INCLUDED_DECL_TYPE_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>
#include <DeclPolicy.hpp>

#include <string>
#include <vector>

namespace srcDispatch {

     struct DeclStmtData {

        unsigned int startLineNumber;
        unsigned int endLineNumber;
        
        std::vector<DeltaElement<std::shared_ptr<DeclData>>> decls;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            std::string str;
            for(const DeltaElement<std::shared_ptr<DeclData>>& decl : decls) {
                str += decl.ToString(operation);
            }
            return str;
        }
    };

    class DeclStmtPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        DeclStmtData data;

        std::unique_ptr<DeclPolicy> declPolicy;

    public:
        DeclStmtPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeDeclStmtPolicyHandlers();
        }

        ~DeclStmtPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<DeclStmtData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if (typeid(DeclPolicy) == typeid(*policy)) {
                // not sure how safe GetElement is here
                srcDispatch::DiffOperation operation = ctx.diffStack.back().operation;
                DeltaElement<std::shared_ptr<DeclData>> decl(operation, policy->Data<DeclData>());
                if (data.decls.size() && decl.GetElement()->type->types.empty()) {
                    decl.GetElement()->type     = data.decls.back().GetElement()->type;
                    decl.GetElement()->isStatic = data.decls.back().GetElement()->isStatic;
                }
                data.decls.push_back(decl);
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeDeclStmtPolicyHandlers() {
            using namespace srcDispatch;

            // start of policy
            openEventMap[ParserState::declstmt] = [this](srcSAXEventContext &ctx) {
                if (depth) return;

                depth = ctx.depth;
                data = DeclStmtData{};
                data.startLineNumber = ctx.startLineNumber;
                data.endLineNumber   = ctx.endLineNumber;                    
                CollectDeclHandlers(ctx);
            };

            // end of policy
            closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext &ctx) {
                if (!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                data = DeclStmtData{};
                InitializeDeclStmtPolicyHandlers();
            };
        }

        void CollectDeclHandlers(srcDispatch::srcSAXEventContext& ctx) {
            using namespace srcDispatch;

            openEventMap[ParserState::decl] = [this](srcSAXEventContext &ctx) {
                if (!depth) return;

                if (!declPolicy) {
                    declPolicy = make_unique_policy<DeclPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(declPolicy.get());
            };
        }
    };

}

#endif
