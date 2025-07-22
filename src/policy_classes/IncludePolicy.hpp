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
#include <DeltaElement.hpp>

#include <string>
#include <iostream>

namespace srcDispatch {

    struct IncludeData {

        DeltaElement<unsigned int> startLineNumber;
        DeltaElement<unsigned int> endLineNumber;

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

            str += path.ToString();

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
                data.startLineNumber = ctx.startLineNumber;
                data.endLineNumber   = ctx.endLineNumber;
                CollectIncludeHandlers();
            };

            // end of policy
            closeEventMap[ParserState::cppinclude] = [this](srcSAXEventContext& ctx) {
                if(!depth || depth != ctx.depth) return;

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
                        bool isChange = (ctx.depth + 1) == ctx.diffStack.back().depth && ctx.diffStack.back().isReplace;
                        srcDispatch::DiffOperation operation = isChange? CHANGE : ctx.diffStack.back().operation;

                        data.path       = DeltaElement<std::string>(operation);
                        data.isRelative = DeltaElement<bool>(operation);
                    }

                    if(    ctx.diffStack.back().operation == srcDispatch::COMMON
                        || ctx.diffStack.back().operation == srcDispatch::DELETE) {
                        data.path.GetOriginal() += ctx.currentToken;
                    } else if(ctx.diffStack.back().operation == srcDispatch::INSERT) {
                        data.path.GetModified() += ctx.currentToken;
                    }
                };
            };
            closeEventMap[ParserState::cppfile] = [this](srcSAXEventContext& ctx) {
                NopCloseEvents({ParserState::tokenstring});

                if(data.path.GetElement().empty()) return;

                if(data.path.HasOriginal() && !data.path.GetOriginal().empty()) {
                    if(data.path.GetOriginal()[0] == '"') {
                        data.isRelative.SetOriginal(true);
                    } else {
                        data.isRelative.SetOriginal(false);
                    }

                    std::size_t length = data.path.GetOriginal().size();
                    data.path.SetOriginal(data.path.GetOriginal().substr(1, length - 2));
                }

                if(data.path.HasModified() && !data.path.GetModified().empty()) {
                    if(data.path.GetModified()[0] == '"') {
                        data.isRelative.SetModified(true);
                    } else {
                        data.isRelative.SetModified(false);
                    }

                    std::size_t length = data.path.GetModified().size();
                    data.path.SetModified(data.path.GetModified().substr(1, length - 2));
                }
            };
        }
    };

}

#endif
