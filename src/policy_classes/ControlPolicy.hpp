// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ControlPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CONTROL_POLICY_HPP
#define INCLUDED_CONTROL_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <DeclPolicy.hpp>
#include <InitPolicy.hpp>
#include <ConditionPolicy.hpp>
#include <IncrPolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct ControlData : public ElementData {

        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

        DeltaElement<std::shared_ptr<InitData>>      init;
        DeltaElement<std::shared_ptr<ConditionData>> condition;
        DeltaElement<std::shared_ptr<IncrData>>      incr;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {

            std::string str = init.ToString(operation);

            if(condition) {
                if(condition.IsOfOperation(operation)) {
                    str += "; ";
                }

                str += condition.ToString(operation);
                if(condition.IsOfOperation(operation)) {
                    str += "; ";
                }
            }

            str += incr.ToString(operation);

            return str;
        }
    };

    // Collect the expression in the return
    //
    class ControlPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ControlData data;

        std::unique_ptr<DeclPolicy>      declPolicy;
        std::unique_ptr<InitPolicy>      initPolicy;
        std::unique_ptr<ConditionPolicy> conditionPolicy;
        std::unique_ptr<IncrPolicy>      incrPolicy;

    public:
        ControlPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeControlPolicyHandlers();
        }

        ~ControlPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<ControlData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            using namespace srcDispatch;
            if(typeid(InitPolicy) == typeid(*policy)) {
                data.init.Update(ctx.diffStack.back().operation, policy->Data<InitData>());
            } else if(typeid(ConditionPolicy) == typeid(*policy)) {
                data.condition = DeltaElement(ctx.diffStack.back().operation, policy->Data<ConditionData>());
            } else if(typeid(IncrPolicy) == typeid(*policy)) {
                data.incr.Update(ctx.diffStack.back().operation, policy->Data<IncrData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }

            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeControlPolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            openEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = ControlData{};
                data.startLineNumber = ctx.startLineNumber;
                data.endLineNumber   = ctx.endLineNumber;
                CollectInitHandlers();
                CollectConditionHandlers();
                CollectIncrHandlers();
            };

            // end of policy
            closeEventMap[ParserState::control] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth)

                depth = 0;
                NotifyAll(ctx);
                InitializeControlPolicyHandlers();
            };
        }

        void CollectInitHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!initPolicy) {
                    initPolicy = make_unique_policy<InitPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(initPolicy.get());
            };
        }

        void CollectConditionHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::condition] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!conditionPolicy) {
                    conditionPolicy = make_unique_policy<ConditionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(conditionPolicy.get());
            };
        }

        void CollectIncrHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::incr] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                if(!incrPolicy) {
                    incrPolicy = make_unique_policy<IncrPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(incrPolicy.get());
            };
        }
    };

}

#endif
