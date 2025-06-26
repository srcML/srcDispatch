// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file LabelPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www..org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_LABEL_POLICY_HPP
#define INCLUDED_SRCDIFF_LABEL_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>

#include <NamePolicy.hpp>

#include <string>
#include <vector>
#include <iostream>

namespace srcDispatch {

    struct LabelData {
        DeltaElement<std::shared_ptr<NameData>> name;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            return name.ToString(operation);
        }
    };

    class LabelPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        LabelData data;

        std::unique_ptr<NamePolicy> namePolicy;

    public:
        LabelPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeLabelPolicyHandlers();
        }

        ~LabelPolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<LabelData>(data); }

        virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
            if(typeid(NamePolicy) == typeid(*policy)) {
                data.name.Update(ctx.diffStack.back().operation, policy->Data<NameData>());
            } else {
                throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
            }
            ctx.dispatcher->RemoveListener(nullptr);
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeLabelPolicyHandlers() {
            using namespace srcDispatch;
            // start of policy
            openEventMap[ParserState::label] = [this](srcSAXEventContext &ctx) {
                if(!depth) {
                    depth = ctx.depth;
                    data = LabelData{};
                    CollectNameHandlers();
                }
            };

            // end of policy
            closeEventMap[ParserState::label] = [this](srcSAXEventContext &ctx) {
                if(!depth || depth != ctx.depth) return;

                depth = 0;
                NotifyAll(ctx);
                InitializeLabelPolicyHandlers();
            };
        }

        void CollectNameHandlers() {
            using namespace srcDispatch;
            openEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
                if(!depth) return;

                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            };
        }
    };

}

#endif
