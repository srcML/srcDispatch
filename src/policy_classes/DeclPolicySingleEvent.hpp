/**
 * @file DeclPolicySingleEvent.hpp
 *
 *
 * MODIFIED from srcDispatch
 *  This collects the initializer
 *
 */
#ifndef INCLUDED_DECL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DECL_POLICY_SINGLE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <NamePolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>

struct DeclData {

    unsigned int lineNumber;
    std::shared_ptr<TypeData>       type;
    std::shared_ptr<NameData>       name;
    std::shared_ptr<ExpressionData> init;
    std::shared_ptr<ExpressionData> range;
    bool                            isStatic;

    friend std::ostream& operator<<(std::ostream& out, const DeclData& declData) {
        if(declData.type) {
            out << declData.type->ToString();
        }
        if (declData.name) {
            out << ' ' << *declData.name;
        }
        if (declData.init) {
            out << " = " << *declData.init;
        }
        if (declData.range) {
            out << " : " << *declData.range;
        }
        return out;
    }
};



class DeclPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    DeclData                          data;
    std::size_t                       declDepth;
    std::unique_ptr<TypePolicy>       typePolicy;
    std::unique_ptr<NamePolicy>       namePolicy;
    std::unique_ptr<ExpressionPolicy> exprPolicy;

public:
    DeclPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          declDepth(0) {
        InitializeDeclPolicyHandlers();
    }

    ~DeclPolicy() {}

protected:
    std::any DataInner() const override { return std::make_shared<DeclData>(data); }

    virtual void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) override {
        using namespace srcDispatch;
        if (typeid(TypePolicy) == typeid(*policy)) {
            data.type = std::shared_ptr<TypeData>(policy->Data<TypeData>());
        } else if (typeid(NamePolicy) == typeid(*policy)) {
            data.name = policy->Data<NameData>(); 
        } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
            if(ctx.IsOpen(ParserState::range)) {
                data.range = policy->Data<ExpressionData>();
            } else {
                data.init = policy->Data<ExpressionData>();
            }
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }

        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeDeclPolicyHandlers() {
        using namespace srcDispatch;

        // start of policy
        openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
            if(declDepth) return;

            declDepth = ctx.depth;
            data = DeclData{};
            data.lineNumber = ctx.startLineNumber;

            CollectSpecifiersHandlers();
            CollectTypeHandlers();
            CollectNameHandlers();
            CollectInitHandlers();
            CollectRangeHandlers();
        };

        // close policy
        closeEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || declDepth != ctx.depth) return;

            declDepth = 0;
            NotifyAll(ctx);
            data = DeclData{};
            InitializeDeclPolicyHandlers();
        };

    }

    void CollectSpecifiersHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                if (ctx.currentToken == "static")
                    data.isStatic = true;
            };
        };
        closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            NopCloseEvents({ParserState::tokenstring});
        };
    }

    void CollectTypeHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            if (!typePolicy) typePolicy = make_unique_policy<TypePolicy>({this});
            ctx.dispatcher->AddListenerDispatch(typePolicy.get());
        };
    }

    void CollectNameHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;
 
            if (!namePolicy) namePolicy = make_unique_policy<NamePolicy>({this});
            ctx.dispatcher->AddListenerDispatch(namePolicy.get());
        };
    }

    void CollectInitHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if(!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        };
        closeEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            NopOpenEvents({ParserState::expr});
        };
    }

    void CollectRangeHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::range] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
                if(!exprPolicy) exprPolicy = make_unique_policy<ExpressionPolicy>({this});
                ctx.dispatcher->AddListenerDispatch(exprPolicy.get());
            };
        };
        closeEventMap[ParserState::range] = [this](srcSAXEventContext& ctx) {
            if(!declDepth || (declDepth + 1) != ctx.depth) return;

            NopOpenEvents({ParserState::expr});
        };
    }


};

#endif
