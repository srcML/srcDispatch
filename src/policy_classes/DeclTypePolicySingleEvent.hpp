/**
 * @file DeclTypePolicySingleEvent.hpp
 *
 *
 * MODIFIED from srcDispatch
 *  This collects the initializer
 *
 */
#ifndef INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <DeclPolicySingleEvent.hpp>

#include <string>
#include <vector>

class DeclTypePolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    std::vector<std::shared_ptr<DeclData>> decls;
    std::size_t                            declDepth;
    DeclPolicy*                            declPolicy;

public:
    DeclTypePolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          decls{},
          declDepth(0),
          declPolicy(nullptr) {
        InitializeDeclTypePolicyHandlers();
    }

    ~DeclTypePolicy() {
        if (declPolicy) delete declPolicy;
    }

protected:
    std::any DataInner() const override { return std::make_shared<std::vector<std::shared_ptr<DeclData>>>(decls); }

    void NotifyWrite(const PolicyDispatcher* policy, srcDispatch::srcSAXEventContext& ctx) override {} //doesn't use other parsers

    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        if (typeid(DeclPolicy) == typeid(*policy)) {
            std::shared_ptr<DeclData> decl = policy->Data<DeclData>();
            if(decls.size()) {
                decl->type     = decls.back()->type;
                decl->isStatic = decls.back()->isStatic;
            }
            decls.push_back(decl);
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

private:
    void InitializeDeclTypePolicyHandlers() {
        using namespace srcDispatch;

        // start of policy
        std::function<void (srcSAXEventContext& ctx)> startDeclType = [this](srcSAXEventContext& ctx) {
            if (!declDepth) {
                declDepth = ctx.depth;
                CollectDeclHandlers();
            }
        };


        openEventMap[ParserState::declstmt]  = startDeclType;
        openEventMap[ParserState::parameter] = startDeclType;

        // end of policy
        std::function<void (srcSAXEventContext& ctx)> endDeclType =  [this](srcSAXEventContext& ctx) {
            if (!declDepth || declDepth != ctx.depth) return;

            declDepth = 0;
            NotifyAll(ctx);
            decls.clear();
            InitializeDeclTypePolicyHandlers();
        };

        closeEventMap[ParserState::declstmt]  = endDeclType;
        closeEventMap[ParserState::parameter] = endDeclType;
    }

    void CollectDeclHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            if (!declPolicy) declPolicy = new DeclPolicy{this};
            ctx.dispatcher->AddListenerDispatch(declPolicy);
        };
    }

};

#endif
