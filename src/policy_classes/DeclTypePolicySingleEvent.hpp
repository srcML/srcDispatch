#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <NamePolicySingleEvent.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP

class DeclTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct DeclTypeData {

        TypePolicy::TypeData * type;
        NamePolicy::NameData * name;
        bool isStatic;

        friend std::ostream & operator<<(std::ostream & out, const DeclTypeData & declData) {

            out << *declData.type;

            if(declData.name)
                out << ' ' << *declData.name;

            return out;

        }

    };

private:


    DeclTypeData data;
    std::size_t declDepth;

    TypePolicy * typePolicy;
    NamePolicy * namePolicy;

public:

    DeclTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          declDepth(0),
          typePolicy(nullptr),
          namePolicy(nullptr) { 
    
        InitializeDeclTypePolicyHandlers();

    }

    ~DeclTypePolicy() {

        if(typePolicy) delete typePolicy;
        if(namePolicy) delete namePolicy;

    }

protected:
    void * DataInner() const override {

        return new DeclTypeData(data);

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

    void InitializeDeclTypePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

            if(!declDepth) {

                declDepth = ctx.depth;
                data = DeclTypeData{};

                CollectTypeHandlers();
                CollectNameHandlers();
                CollectSpecifiersHandlers();

            }

        };

        // end of policy
        closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

            if(declDepth && declDepth == ctx.depth) {

                declDepth = 0;
 
                NotifyAll(ctx);
                InitializeDeclTypePolicyHandlers();

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



void CollectSpecifiersHandlers() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

        if(declDepth && (declDepth + 2) == ctx.depth) {

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                if(ctx.currentToken == "static")
                    data.isStatic = true;

            };

        }

    };

    closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

        if(declDepth && (declDepth + 1) == ctx.depth) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

}

};

#endif
