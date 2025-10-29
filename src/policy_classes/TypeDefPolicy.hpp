// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TypeDefPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_TYPE_DEF_POLICY_HPP
#define INCLUDED_TYPE_DEF_POLICY_HPP

#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <TypePolicy.hpp>
#include <NamePolicy.hpp>
#include <FunctionPolicy.hpp>

#include <string>
#include <vector>

namespace srcDispatch {

     struct TypeDefData : public ElementData {

        DeltaElement<std::shared_ptr<TypeData>> type;
        DeltaElement<std::shared_ptr<NameData>> name;

        // Consider merging with type and extracting name out of it
        DeltaElement<std::shared_ptr<FunctionData>> functionDecl;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            std::string str;
            return str;
        }
    };

    class TypeDefPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        TypeDefData data;

        std::unique_ptr<TypePolicy> typePolicy;
        std::unique_ptr<NamePolicy> namePolicy;

        std::unique_ptr<FunctionPolicy> functionPolicy;

    public:
        TypeDefPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeTypeDefPolicyHandlers();
        }

        ~TypeDefPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<TypeDefData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(TypePolicy) == typeid(*policy)) {
                data.type = DeltaElement(ctx.diffStack.back().operation, policy->Data<TypeData>());
            } else if(typeid(NamePolicy) == typeid(*policy)) {
                data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
            } else if(typeid(FunctionPolicy) == typeid(*policy)) {
                data.functionDecl = DeltaElement(ctx.diffStack.back().operation, policy->Data<FunctionData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeTypeDefPolicyHandlers() {
            using namespace srcDispatch;

            // start of policy
            openEventMap[ParserState::typedefdecl] = [this](srcSAXEventContext &ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = TypeDefData{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;                    
                CollectTypeHandlers(ctx);
            };

            // end of policy
            closeEventMap[ParserState::typedefdecl] = [this](srcSAXEventContext &ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                data = TypeDefData{};
                InitializeTypeDefPolicyHandlers();
            };
        }

        void CollectTypeHandlers(srcDispatch::srcSAXEventContext& ctx) {
            using namespace srcDispatch;

            openEventMap[ParserState::type] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!typePolicy) {
                    typePolicy = make_unique_policy<TypePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(typePolicy.get());
            };
        }

        void CollectTypeHandlers(srcDispatch::srcSAXEventContext& ctx) {
            using namespace srcDispatch;

            openEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        }

        void CollectTypeHandlers(srcDispatch::srcSAXEventContext& ctx) {
            using namespace srcDispatch;

            openEventMap[ParserState::functiondecl] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!functionPolicy) {
                    functionPolicy = make_unique_policy<FunctionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(functionPolicy.get());
            };
        }
    };

}

#endif
