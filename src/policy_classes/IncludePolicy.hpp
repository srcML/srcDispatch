// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file IncludePolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_INCLUDE_POLICY_HPP
#define INCLUDED_INCLUDE_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <string>
#include <iostream>

namespace srcDispatch {

    struct IncludeData : public ElementData {

        DeltaElement<bool> isRelative;
        DeltaElement<std::string> path;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            std::string str;
            if(isRelative.IsOfOperation(operation)) {
                str += std::string("#include ") + (isRelative.GetOfOperation(operation)? '"' : '<');
            }

            str += path.ToString(operation);

            if(isRelative.IsOfOperation(operation)) {
                str += isRelative.GetOfOperation(operation)? '"' : '>';
            }

            return str;
        }
    };

    class IncludePolicy :
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        IncludeData data;

    public:
        IncludePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
            : srcDispatch::PolicyDispatcher(listeners), data{} {
            InitializeIncludePolicyHandlers();
        }

        ~IncludePolicy() {}

    protected:
        std::any DataInner() const override { return std::make_shared<IncludeData>(data); }
        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {}
        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    private:
        void InitializeIncludePolicyHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::cppinclude] = [this](srcSAXEventContext& ctx) {
                if(depth) return;

                depth = ctx.depth;
                data = IncludeData{};
                data.startPosition = ctx.startPosition;
                data.endPosition   = ctx.endPosition;
                CollectIncludeHandlers();
            };

            // end of policy
            closeEventMap[ParserState::cppinclude] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

                std::optional<std::string> originalPath;
                std::optional<bool>        originalIsRelative;
                if(!data.path.GetOriginal().empty()) {
                    originalIsRelative = data.path.GetOriginal()[0] == '"';

                    std::size_t length = data.path.GetOriginal().size();
                    originalPath       = data.path.GetOriginal().substr(1, length - 2);
                }

                std::optional<std::string> modifiedPath;
                std::optional<bool>        modifiedIsRelative;
                if(!data.path.GetModified().empty()) {
                    modifiedIsRelative = data.path.GetModified()[0] == '"';

                    std::size_t length = data.path.GetModified().size();
                    modifiedPath       = data.path.GetModified().substr(1, length - 2);
                }

                data.path       = DeltaElement(originalPath,       modifiedPath);
                data.isRelative = DeltaElement(originalIsRelative, modifiedIsRelative);

                depth = 0;
                NotifyAll(ctx);
                InitializeIncludePolicyHandlers();
            };
        }

        void CollectIncludeHandlers() {
            using namespace srcDispatch;

            openEventMap[ParserState::cppfile] = [this](srcSAXEventContext& ctx) {
                if(!depth) return;

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                    if(data.path.GetOperation() == srcDispatch::NONE) {
                        data.path = DeltaElement<std::string>(srcDispatch::CHANGE);
                    }

                    if(ctx.diffStack.back().operation == srcDispatch::COMMON) {
                        data.path.GetOriginal() += ctx.currentToken;
                        data.path.GetModified() += ctx.currentToken;
                    } else if(ctx.diffStack.back().operation == srcDispatch::DELETE) {
                        data.path.GetOriginal() += ctx.currentToken;
                    } else if(ctx.diffStack.back().operation == srcDispatch::INSERT) {
                        data.path.GetModified() += ctx.currentToken;
                    }
                };
            };
            closeEventMap[ParserState::cppfile] = [this](srcSAXEventContext& ctx) {
                NopCloseEvents({ParserState::tokenstring});
            };
        }
    };

}

#endif
