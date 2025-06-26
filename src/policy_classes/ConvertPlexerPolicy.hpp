// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ConvertPlexerPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www..org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CONVERT_PLEXER_POLICY_HPP
#define INCLUDED_CONVERT_PLEXER_POLICY_HPP

#include <srcSAXController.hpp>
#include <srcDispatcher.hpp>
#include <srcDispatchUtilities.hpp>

#include <DeltaElement.hpp>

#include <IfStmtPolicy.hpp>
#include <WhilePolicy.hpp>
#include <ForPolicy.hpp>

#include <DeclStmtPolicy.hpp>
#include <ExprStmtPolicy.hpp>
#include <ReturnPolicy.hpp>

#include <string>
#include <vector>
#include <stack>
#include <iostream>

// #define CONVERT_DEBUG

namespace srcDispatch {

    struct ConvertData {
        DeltaElement<std::any> construct;
    };

    class ConvertPlexerPolicy :
    public srcDispatch::EventListener,
    public srcDispatch::EventDispatcher,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    protected:
        ConvertData data;

        std::stack<EventDispatcher*> dispatcherStack;

        std::unique_ptr<EventListener> originalPolicy;
        std::unique_ptr<EventListener> modifiedPolicy;

        std::stack<srcDispatch::DiffOperation> dispatchingState;

        std::stack<EventListener*> originalPolicyStack;
        std::stack<EventListener*> modifiedPolicyStack;
public:
        static std::size_t s_index;
        std::size_t index;
protected:
    static std::unordered_map<srcDispatch::ParserState, 
                              std::function<std::unique_ptr<srcDispatch::EventListener>(std::initializer_list<srcDispatch::PolicyListener *> listeners)>
                             > policyFactory;
    public:
        ConvertPlexerPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
            : srcDispatch::EventDispatcher({}), srcDispatch::PolicyDispatcher(listeners), data(), 
              dispatcherStack(), dispatchingState(), originalPolicy(), modifiedPolicy(), originalPolicyStack(), modifiedPolicyStack() {
            dispatchingState.push(srcDispatch::NONE);
            index = ++s_index;
        }

        ~ConvertPlexerPolicy() {}

        void Stop() {
            dispatched = true;
            EventDispatcher* currentDispatcher = dispatcherStack.top()->GetContext().dispatcher;
            dispatcherStack.top()->GetContext().dispatcher = dispatcherStack.top();
            NotifyAll(dispatcherStack.top()->GetContext());
            dispatcherStack.top()->GetContext().dispatcher = currentDispatcher;
        }

        virtual void AddListener(EventListener* listener) override {
#ifdef CONVERT_DEBUG            
            std::cerr << "ADD: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' << dispatchingState.top() << '\n';
#endif
            if(dispatchingState.top() == srcDispatch::DELETE) {
#ifdef CONVERT_DEBUG 
                std::cerr << "ADD: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << '\n';
#endif
                originalPolicyStack.top()->SetDispatched(false);
                originalPolicyStack.push(listener);
            } else if(dispatchingState.top() == srcDispatch::INSERT) {
#ifdef CONVERT_DEBUG 
                std::cerr << "ADD: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << '\n';
#endif
                modifiedPolicyStack.top()->SetDispatched(false);
                modifiedPolicyStack.push(listener);
            }
        }

        virtual void AddListenerDispatch(EventListener* listener) override {
            AddListener(listener);
            dispatched = false;             
        }

        virtual void AddListenerNoDispatch(EventListener* listener) override {
            throw std::string("AddListenerNoDispatch not implemented");
        }

        virtual void RemoveListener(EventListener* listener) override {
            if(dispatchingState.top() == srcDispatch::DELETE) {
#ifdef CONVERT_DEBUG 
                std::cerr << "REMOVE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << '\n';
#endif
                originalPolicyStack.top()->SetDispatched(false);
                originalPolicyStack.pop();
            } else if(dispatchingState.top() == srcDispatch::INSERT) {
#ifdef CONVERT_DEBUG 
                std::cerr << "REMOVE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << '\n';
#endif
                modifiedPolicyStack.top()->SetDispatched(false);
                modifiedPolicyStack.pop();
            }
        }

        virtual void RemoveListenerDispatch(EventListener* listener) override {
            RemoveListener(listener);
            dispatched = false;             
        }

        virtual void RemoveListenerNoDispatch(EventListener* listener) override {
            throw std::string("RemoveListenerNoDispatch not implemented");
        }

        virtual void DispatchEvent(srcDispatch::ParserState, srcDispatch::ElementState) override {
        }

        virtual void HandleEvent(srcDispatch::ParserState pstate, srcDispatch::ElementState estate, srcDispatch::srcSAXEventContext& ctx) override {
#ifdef CONVERT_DEBUG 
            std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  pstate << ":" << estate << '\n';
            std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  ctx.diffStack.back().operation << '\n';
            std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  ctx.currentTag << '\n';
            std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  ctx.depth << '\n';
            std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  depth << '\n';
#endif

            dispatcherStack.push(ctx.dispatcher);
            ctx.dispatcher = this;
            while(!dispatched) {

                dispatched = true;

                if(    dispatchingState.size() && dispatchingState.top() != srcDispatch::INSERT 
                    && (ctx.diffStack.back().operation != srcDispatch::INSERT || (depth && ctx.depth > depth))) {

                    if(!originalPolicy) {
                        originalPolicy = policyFactory[pstate]({this});
                        originalPolicyStack.push(originalPolicy.get());
                    }

#ifdef CONVERT_DEBUG 
                    std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' << originalPolicyStack.size() << '\n';
#endif

                    dispatchingState.push(srcDispatch::DELETE);
                    originalPolicyStack.top()->HandleEvent(pstate, estate, ctx);
                    dispatchingState.pop();
                    if(dispatchingState.size() == 1) {
                        originalPolicyStack.top()->SetDispatched(false);
                    }
                }

#ifdef CONVERT_DEBUG 
                std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << '\n';
#endif

                if(    dispatchingState.size() && dispatchingState.top() != srcDispatch::DELETE
                    && (ctx.diffStack.back().operation != srcDispatch::DELETE || (depth && ctx.depth > depth))) {

                    if(!modifiedPolicy) {
                        modifiedPolicy = policyFactory[pstate]({this});
                        modifiedPolicyStack.push(modifiedPolicy.get());

                        if(pstate != srcDispatch::ParserState::ifgroup) {
                            depth = ctx.diffStack.back().depth + 1;
                        } else {
                            depth = ctx.diffStack.back().depth + 2;
                        }
                    }

#ifdef CONVERT_DEBUG 
                    std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' << modifiedPolicyStack.size() << '\n';
#endif

                    dispatchingState.push(srcDispatch::INSERT);
                    modifiedPolicyStack.top()->HandleEvent(pstate, estate, ctx);
                    dispatchingState.pop();
                    if(dispatchingState.size() == 1) {
                        modifiedPolicyStack.top()->SetDispatched(false);
                    }
                }
            }
            ctx.dispatcher = dispatcherStack.top();
            dispatcherStack.pop();
        }

    protected:
        std::any DataInner() const override { return std::make_shared<ConvertData>(data); }

        void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
#ifdef CONVERT_DEBUG 
            std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  '\n';
#endif

            std::any anyData;
            if(typeid(WhilePolicy) == typeid(*policy)) {
                anyData = policy->Data<WhileData>();
            } else if(typeid(ForPolicy) == typeid(*policy)) {
                anyData = policy->Data<ForData>();
            } else if(typeid(IfStmtPolicy) == typeid(*policy)) {
                anyData = policy->Data<IfStmtData>();
            } else if(typeid(DeclStmtPolicy) == typeid(*policy)) {
                anyData = policy->Data<DeclStmtData>();
            } else if(typeid(ExprStmtPolicy) == typeid(*policy)) {
                anyData = policy->Data<ExprStmtData>();
            } else if(typeid(ReturnPolicy) == typeid(*policy)) {
                anyData = policy->Data<ReturnData>();
            }

            data.construct.Update(ctx.diffStack.back().operation, anyData);
            if(data.construct.HasOriginal() && data.construct.HasModified()) {

#ifdef CONVERT_DEBUG 
                std::cerr << "HERE: " << std::string(index * 4, ' ') << index << ' ' << __LINE__ << ' ' <<  '\n';
#endif

                Stop();
            }
        }

        void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    };

    std::unordered_map<srcDispatch::ParserState, 
                              std::function<std::unique_ptr<srcDispatch::EventListener>(std::initializer_list<srcDispatch::PolicyListener *> listeners)>
                      >ConvertPlexerPolicy::policyFactory = {
        { srcDispatch::ParserState::ifgroup,   [](std::initializer_list<srcDispatch::PolicyListener *> listeners) { return srcDispatch::make_unique_policy<IfStmtPolicy>(listeners); } },
        { srcDispatch::ParserState::whilestmt, [](std::initializer_list<srcDispatch::PolicyListener *> listeners) { return srcDispatch::make_unique_policy<WhilePolicy> (listeners); } },
        { srcDispatch::ParserState::forstmt,   [](std::initializer_list<srcDispatch::PolicyListener *> listeners) { return srcDispatch::make_unique_policy<ForPolicy> (listeners); } },

        { srcDispatch::ParserState::declstmt,     [](std::initializer_list<srcDispatch::PolicyListener *> listeners) { return srcDispatch::make_unique_policy<DeclStmtPolicy> (listeners); } },
        { srcDispatch::ParserState::exprstmt,     [](std::initializer_list<srcDispatch::PolicyListener *> listeners) { return srcDispatch::make_unique_policy<ExprStmtPolicy> (listeners); } },
        { srcDispatch::ParserState::returnstmt,   [](std::initializer_list<srcDispatch::PolicyListener *> listeners) { return srcDispatch::make_unique_policy<ReturnPolicy>   (listeners); } },

    };

    std::size_t ConvertPlexerPolicy::s_index = 0;
}

#endif
