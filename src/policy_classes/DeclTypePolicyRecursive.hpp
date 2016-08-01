#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>

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

    TypePolicy * typePolicy;
    NamePolicy * namePolicy;

public:


    DeclTypePolicyRecursive(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          declDepth(0),
          typePolicy(nullptr),
          namePolicy(nullptr) { 
    
        InitializeDeclTypePolicyRecursiveHandlers();

    }

protected:
    void * DataInner() const override {

        return new DeclTypeRecursiveData(data);

    }
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(TypePolicy) == typeid(*policy)) {

            data.type = policy->Data<TypePolicy::TypeData>();

        } else if(typeid(NamePolicy) == typeid(*policy)) {

            data.name = policy->Data<NamePolicy::NameData>(); 

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

                typePolicy = new TypePolicy{this};
                ctx.AddListenerDispatch(typePolicy);

            }

        };

        closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 1) == ctx.depth) {

                if(typePolicy) {
                    ctx.RemoveListenerDispatch(typePolicy);
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

                namePolicy = new NamePolicy{this};
                ctx.AddListenerDispatch(namePolicy);

            }

        };

        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 1) == ctx.depth) {

                if(namePolicy) {
                    ctx.RemoveListenerDispatch(namePolicy);
                    delete namePolicy;
                    namePolicy = nullptr;
                }

            }

        };

    }

};

#endif
