#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>

#include <SingleEventPolicyDispatcher.hpp>

#include <TypePolicy.hpp>
#include <NamePolicy.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_DECL_TYPE_POLICY_RECURSIVE_HPP
#define INCLUDED_DECL_TYPE_POLICY_RECURSIVE_HPP

class DeclTypePolicyRecursive : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct DeclTypeRecursiveData {

        TypePolicy::TypeData * type;
        NamePolicy::NameData * name;

        friend std::ostream & operator<<(std::ostream & out, const DeclTypeRecursiveData & nameData) {
            return out;
        }

    };

private:


    DeclTypeRecursiveData data;
    std::size_t declDepth;

    SingleEventPolicyDispatcher & policy_handler;

    TypePolicy * typePolicy;
    NamePolicy * namePolicy;

public:

    DeclTypePolicyRecursive(SingleEventPolicyDispatcher & policy_handler, std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          policy_handler(policy_handler),
          data{},
          declDepth(0),
          typePolicy(nullptr),
          namePolicy(nullptr) { 
    
        InitializeDeclTypePolicyRecursiveHandlers();

    }

protected:
    void * DataInner() const override {

        return nullptr;//new DeclTypeRecursiveData(data);

    }
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(TypePolicy) == typeid(*policy)) {

            data.type = policy->Data<TypePolicy::TypeData>();
            policy_handler.PopListenerDispatch();

        } else if(typeid(NamePolicy) == typeid(*policy)) {

            data.name = policy->Data<NamePolicy::NameData>(); 
            policy_handler.PopListenerDispatch();

        }

    }

private:

    void InitializeDeclTypePolicyRecursiveHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

            if(!declDepth) {

                declDepth = ctx.depth;
                data = DeclTypeRecursiveData{};

                CollectTypeHandlers();
                CollectNameHandlers();

            }

        };

        // end of policy
        closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

            if(declDepth && declDepth == ctx.depth) {

                declDepth = 0;
 
                NotifyAll(ctx);
                InitializeDeclTypePolicyRecursiveHandlers();

            }
           
        };

    }

    void CollectTypeHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                typePolicy = new TypePolicy(policy_handler, {this});
                policy_handler.PushListenerDispatch(typePolicy);

            }

        };

        closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                if(typePolicy) {
                    delete typePolicy;
                    typePolicy = nullptr;
                }

            }

        };

    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                namePolicy = new NamePolicy(policy_handler, {this});
                policy_handler.PushListenerDispatch(namePolicy);

            }

        };

        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                if(namePolicy) {
                    delete namePolicy;
                    namePolicy = nullptr;
                }

            }

        };

    }

};

#endif
