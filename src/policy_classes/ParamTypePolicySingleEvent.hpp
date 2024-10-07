/**
 * @file ParamTypePolicySingleEvent.hpp
 *
  */

#ifndef INCLUDED_PARAM_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_PARAM_TYPE_POLICY_SINGLE_EVENT_HPP


#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <NamePolicySingleEvent.hpp>

#include <string>
#include <vector>

struct ParamTypeData {

    unsigned int lineNumber;
    std::shared_ptr<TypeData> type;
    std::shared_ptr<NameData> name;

    friend std::ostream & operator<<(std::ostream& out, const ParamTypeData& paramData) {
        out << *paramData.type;
        if (paramData.name)
            out << ' ' << *paramData.name;
        return out;
    }
};


class ParamTypePolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
    ParamTypeData data;
    std::size_t paramDepth;
    TypePolicy* typePolicy;
    NamePolicy* namePolicy;

public:
    ParamTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener*> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          paramDepth(0),
          typePolicy(nullptr),
          namePolicy(nullptr) { 

        InitializeParamTypePolicyHandlers();
    }

    ~ParamTypePolicy() {
        if (typePolicy) delete typePolicy;
        if (namePolicy) delete namePolicy;

    }

protected:
    std::any DataInner() const override { return std::make_shared<ParamTypeData>(data); }

    virtual void Notify(const PolicyDispatcher* policy, const srcSAXEventDispatch::srcSAXEventContext& ctx) override {
        if (typeid(TypePolicy) == typeid(*policy)) {
            data.type = policy->Data<TypeData>();
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(NamePolicy) == typeid(*policy)) {
            data.name = policy->Data<NameData>(); 
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }
    }

    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcSAXEventDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {} //doesn't use other parsers

private:
    void InitializeParamTypePolicyHandlers() {
        using namespace srcSAXEventDispatch;
        // start of policy
        openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
            if (!paramDepth) {
                paramDepth = ctx.depth;
                data = ParamTypeData{};
                data.lineNumber = ctx.currentLineNumber;
                CollectTypeHandlers();
                CollectNameHandlers();
            }
        };

        // end of policy
        closeEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
            if (paramDepth && paramDepth == ctx.depth) {
                paramDepth = 0;
                NotifyAll(ctx);
                InitializeParamTypePolicyHandlers();
            }
        };
    }

    void CollectTypeHandlers() {
        using namespace srcSAXEventDispatch;
        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if (paramDepth && (paramDepth + 2) == ctx.depth) {
                if (!typePolicy) typePolicy = new TypePolicy{this};
                ctx.dispatcher->AddListenerDispatch(typePolicy);
            }
        };
    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if (paramDepth && (paramDepth + 2) == ctx.depth) {
                if (!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy);
            }
        };
    }
};

#endif
