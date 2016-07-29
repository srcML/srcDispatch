#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>
#include <NamePolicy.hpp>
#include <DeclTypePolicy.hpp>
#include <FunctionSignaturePolicy.hpp>

#include <string>
#include <vector>

class ClassPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    enum ClassType { CLASS, STRUCT/*, UNION, ENUM*/ };
    enum AccessSpecifier { PUBLIC, PRIVATE, PROTECTED };
    struct ParentData {

        std::string name;
        bool isVirtual;
        AccessSpecifier accessSpecifier;

    };

    struct ClassData {

        ClassType type;

        NamePolicy::NameData * name;

        std::vector<ParentData> parents;

        std::vector<DeclTypePolicy> fields;
        std::vector<FunctionSignaturePolicy> methods;

    };

private:

    ClassData data;
    std::size_t classDepth;
    AccessSpecifier currentRegion;

    NamePolicy * namePolicy;

public:


    ClassPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          classDepth(0),
          currentRegion(PUBLIC),
          namePolicy(nullptr) { 
    
        InitializeClassPolicyHandlers();

    }

    void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        data.name = policy->Data<NamePolicy::NameData>();

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

                namePolicy = new NamePolicy{this};
                ctx.AddListenerDispatch(namePolicy);

            }

        };

        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                if(namePolicy) {

                    ctx.RemoveListenerDispatch(namePolicy);
                    delete namePolicy;
                    namePolicy = nullptr;

                }

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

        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::block, ParserState::publicaccess, ParserState::protectedaccess, ParserState::privateaccess});
                NopCloseEvents({ParserState::block});

            }

        };

    }

};
