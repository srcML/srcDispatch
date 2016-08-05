#include <srcSAXEventDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
#include <DeclTypePolicySingleEvent.hpp>
#include <FunctionSignaturePolicySingleEvent.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_CLASS_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CLASS_POLICY_SINGLE_EVENT_HPP

class ClassPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    enum ClassType { CLASS, STRUCT/*, UNION, ENUM*/ };
    enum AccessSpecifier { PUBLIC = 0, PRIVATE = 1, PROTECTED = 2 };
    struct ParentData {

        std::string name;
        bool isVirtual;
        AccessSpecifier accessSpecifier;

    };

    struct ClassData {

        ClassType type;

        NamePolicy::NameData * name;

        std::vector<ParentData> parents;

        std::vector<DeclTypePolicy::DeclTypeData *> fields[3];
        std::vector<FunctionSignaturePolicy::FunctionSignatureData *> methods[3];

    };

private:

    ClassData data;
    std::size_t classDepth;
    AccessSpecifier currentRegion;

    NamePolicy * namePolicy;
    DeclTypePolicy * declPolicy;
    FunctionSignaturePolicy * functionPolicy;

public:

    ClassPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          classDepth(0),
          currentRegion(PUBLIC),
          namePolicy(nullptr),
          declPolicy(nullptr),
          functionPolicy(nullptr) { 
    
        InitializeClassPolicyHandlers();

    }

    ~ClassPolicy() {

        if(namePolicy)     delete namePolicy;
        if(declPolicy)     delete declPolicy;
        if(functionPolicy) delete functionPolicy;

    }

    void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(NamePolicy) == typeid(*policy)) {

            data.name = policy->Data<NamePolicy::NameData>();
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        } else if(typeid(DeclTypePolicy) == typeid(*policy)) {

            data.fields[currentRegion].emplace_back(policy->Data<DeclTypePolicy::DeclTypeData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        } else if(typeid(FunctionSignaturePolicy) == typeid(*policy)) {

            data.methods[currentRegion].emplace_back(policy->Data<FunctionSignaturePolicy::FunctionSignatureData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        }

    }

protected:
    void * DataInner() const override {

        return new ClassData(data);

    }

private:

    void InitializeClassPolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> startPolicy = [this](srcSAXEventContext& ctx) {

            if(!classDepth) {

                classDepth = ctx.depth;

                data = ClassData{};
                if(ctx.elementStack.back() == "class")
                    data.type = CLASS;
                else if(ctx.elementStack.back() == "struct")
                    data.type = STRUCT;

                data.name = nullptr;

                CollectNameHandlers();
                CollectSuperHanders();
                CollectBlockHanders();

            }

        };

        // end of policy
        std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> endPolicy = [this](srcSAXEventContext& ctx) {

            if(classDepth && classDepth == ctx.depth) {

                classDepth = 0;
                NotifyAll(ctx);
                InitializeClassPolicyHandlers();

            }
           
        };

        openEventMap[ParserState::classn] = startPolicy;
        closeEventMap[ParserState::classn] = endPolicy;
        openEventMap[ParserState::structn] = startPolicy;
        closeEventMap[ParserState::structn] = endPolicy;

    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                if(!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy);

            }

        };

        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::name});
                NopCloseEvents({ParserState::name});

            }

        };

    }

    void CollectSuperHanders() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                openEventMap[ParserState::super] = [this](srcSAXEventContext& ctx) {

                    data.parents.emplace_back(ParentData{ "", false, PUBLIC });

                };

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                    if(ctx.And({ ParserState::specifier })) {

                        if(ctx.currentToken == "virtual") {
                            data.parents.back().isVirtual = true;
                        } else if(ctx.currentToken == "public") {
                            data.parents.back().accessSpecifier = PUBLIC;
                        } else if(ctx.currentToken == "private") {
                            data.parents.back().accessSpecifier = PRIVATE;
                        } else if(ctx.currentToken == "protected") {
                            data.parents.back().accessSpecifier = PROTECTED;
                        }

                    } else if(ctx.And({ ParserState::name })) {

                        data.parents.back().name += ctx.currentToken;

                    }


                };              

            }

        };

        closeEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::super_list, ParserState::super});
                NopCloseEvents({ParserState::super_list, ParserState::tokenstring});

            }

        };

    }

    void CollectBlockHanders() {
        using namespace srcSAXEventDispatch;

       openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::name, ParserState::super_list, ParserState::super});
                NopCloseEvents({ParserState::name, ParserState::super_list, ParserState::tokenstring});

                // set up to listen to decl_stmt, member, and class policies
                openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

                    if((classDepth + 3) == ctx.depth) {

                        if(!declPolicy) declPolicy = new DeclTypePolicy{this};
                        ctx.dispatcher->AddListenerDispatch(declPolicy);

                    }

                };
                std::function<void (srcSAXEventContext& ctx)> functionEvent = [this](srcSAXEventContext& ctx) {

                    if((classDepth + 3) == ctx.depth) {

                        if(!functionPolicy) functionPolicy = new FunctionSignaturePolicy{this};
                        ctx.dispatcher->AddListenerDispatch(functionPolicy);

                    }

                };
                openEventMap[ParserState::function] = functionEvent;
                openEventMap[ParserState::functiondecl] = functionEvent;
                openEventMap[ParserState::constructor] = functionEvent;
                openEventMap[ParserState::constructordecl] = functionEvent;
                openEventMap[ParserState::destructor] = functionEvent;
                openEventMap[ParserState::destructordecl] = functionEvent;

            }

        };


        // should always be in a region once block starts, so should not have to close
        openEventMap[ParserState::publicaccess] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 2) == ctx.depth) {

                currentRegion = PUBLIC;

            }

        };

        openEventMap[ParserState::protectedaccess] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 2) == ctx.depth) {

                currentRegion = PROTECTED;

            }

        };

        openEventMap[ParserState::privateaccess] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 2) == ctx.depth) {

                currentRegion = PRIVATE;

            }

        };

        closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::block, ParserState::function, ParserState::functiondecl, 
                               ParserState::constructor, ParserState::constructordecl, ParserState::destructor, ParserState::destructordecl,
                               ParserState::declstmt,
                               ParserState::publicaccess, ParserState::protectedaccess, ParserState::privateaccess});
                NopCloseEvents({ParserState::block});

            }

        };

    }

};

#endif
