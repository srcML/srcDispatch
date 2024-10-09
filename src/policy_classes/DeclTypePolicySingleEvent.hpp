/**
 * @file DeclTypePolicySingleEvent.hpp
 *
 *
 * MODIFIED from srcSAXEventDispatcher
 *  This collects the initializer
 *
 */
#ifndef INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <NamePolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>

#include <string>
#include <vector>

struct DeclTypeData {

    DeclTypeData(unsigned int lineNumber) 
        : lineNumber(lineNumber), type(), name(), initializer(), isStatic() {
    }

    unsigned int lineNumber;
    std::shared_ptr<TypeData>       type;
    std::shared_ptr<NameData>       name;
    std::shared_ptr<ExpressionData> initializer;
    bool                            isStatic;

    friend std::ostream& operator<<(std::ostream& out, const DeclTypeData& declData) {
        out << declData.type->ToString();
        if (declData.name)
            out << ' ' << *declData.name;
        if (declData.initializer)
            out << " = " << *declData.initializer;
        return out;
    }
};



class DeclTypePolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    std::vector<std::shared_ptr<DeclTypeData>> data;
    std::size_t                                declDepth;
    TypePolicy*                                typePolicy;
    NamePolicy*                                namePolicy;
    ExpressionPolicy*                          expressionPolicy;

    bool                                       isStatic;
    std::shared_ptr<TypeData>                  type;
    std::shared_ptr<ExpressionData>            initializer;

public:
    DeclTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          declDepth(0),
          typePolicy(nullptr),
          isStatic(false),
          type(),
          expressionPolicy(nullptr),
          namePolicy(nullptr) {
        InitializeDeclTypePolicyHandlers();
    }

    ~DeclTypePolicy() {
        if (typePolicy)       delete typePolicy;
        if (namePolicy)       delete namePolicy;
        if (expressionPolicy) delete expressionPolicy;
    }

protected:
    std::any DataInner() const override { return std::make_shared<std::vector<std::shared_ptr<DeclTypeData>>>(data); }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcSAXEventDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} //doesn't use other parsers

    virtual void Notify(const PolicyDispatcher* policy, const srcSAXEventDispatch::srcSAXEventContext& ctx) override {
        if (typeid(TypePolicy) == typeid(*policy)) {
            type = std::shared_ptr<TypeData>(policy->Data<TypeData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(NamePolicy) == typeid(*policy)) {
            data.back()->name = policy->Data<NameData>(); 
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
            initializer = policy->Data<ExpressionData>();
            ctx.dispatcher->RemoveListener(nullptr);
        }

    }

private:
    void InitializeDeclTypePolicyHandlers() {
        using namespace srcSAXEventDispatch;
        // start of policy
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if (!declDepth) {
                declDepth = ctx.depth;
                CollectTypeHandlers();
                CollectNameHandlers();
                CollectSpecifiersHandlers();
                CollectInitHandlers();
            }
            openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
                if (declDepth && (declDepth + 1) == ctx.depth) {
                    data.push_back(std::make_shared<DeclTypeData>(ctx.currentLineNumber));
                }
            };
            closeEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {
                if (declDepth && (declDepth + 1) == ctx.depth) {
                    data.back()->isStatic = isStatic;
                    data.back()->type = type;
                    data.back()->initializer = initializer;
                }
            };
        };

        // end of policy
        closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            if (declDepth && declDepth == ctx.depth) {
                declDepth = 0;
                NotifyAll(ctx);
                InitializeDeclTypePolicyHandlers();
            }
        };
    }

    void CollectTypeHandlers() {
        using namespace srcSAXEventDispatch;
        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if (declDepth && (declDepth + 2) == ctx.depth) {
                if (!typePolicy) typePolicy = new TypePolicy{this};
                ctx.dispatcher->AddListenerDispatch(typePolicy);
            }
        };
    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if (declDepth && (declDepth + 2) == ctx.depth) {
                if (!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy);
            }
        };
    }

    void CollectSpecifiersHandlers() {
        using namespace srcSAXEventDispatch;
        openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
            if (declDepth && (declDepth + 2) == ctx.depth) {
                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                    if (ctx.currentToken == "static")
                        isStatic = true;
                };
            }
        };
        closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
            if (declDepth && (declDepth + 1) == ctx.depth) {
                NopCloseEvents({ParserState::tokenstring});
            }
        };
    }

    void CollectInitHandlers() {
        using namespace srcSAXEventDispatch;
        openEventMap[ParserState::init] = [this](srcSAXEventContext& ctx) {
            if(!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(expressionPolicy);
        };
    }

};

#endif
