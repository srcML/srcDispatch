#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <NamePolicySingleEvent.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_PARAM_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_PARAM_TYPE_POLICY_SINGLE_EVENT_HPP

class ParamTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct ParamTypeData {

        TypePolicy::TypeData * type;
        NamePolicy::NameData * name;

        friend std::ostream & operator<<(std::ostream & out, const ParamTypeData & nameData) {
            return out;
        }

    };

private:


    ParamTypeData data;
    std::size_t declDepth;

    TypePolicy * typePolicy;
    NamePolicy * namePolicy;

public:

    ParamTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          declDepth(0),
          typePolicy(nullptr),
          namePolicy(nullptr) { 
    
        InitializeParamTypePolicyHandlers();

    }

    ~ParamTypePolicy() {

        if(typePolicy) delete typePolicy;
        if(namePolicy) delete namePolicy;

    }

protected:
    void * DataInner() const override {

        return new ParamTypeData(data);

    }
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(TypePolicy) == typeid(*policy)) {

            data.type = policy->Data<TypePolicy::TypeData>();
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        } else if(typeid(NamePolicy) == typeid(*policy)) {

            data.name = policy->Data<NamePolicy::NameData>(); 
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        }

    }

private:

    void InitializeParamTypePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {

            if(!declDepth) {

                declDepth = ctx.depth;
                data = ParamTypeData{};

                CollectTypeHandlers();
                CollectNameHandlers();

            }

        };

        // end of policy
        closeEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {

            if(declDepth && declDepth == ctx.depth) {

                declDepth = 0;
 
                NotifyAll(ctx);
                InitializeParamTypePolicyHandlers();

            }
           
        };

    }

    void CollectTypeHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                if(!typePolicy) typePolicy = new TypePolicy{this};
                ctx.dispatcher->AddListenerDispatch(typePolicy);

            }

        };

    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                if(!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy);

            }

        };

    }

};

#endif
