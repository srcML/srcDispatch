#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>
#include <TypePolicy.hpp>
#include <FunctionSignaturePolicy.hpp>

#include <string>
#include <vector>

class ClassPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher {

public:

    enum AccessSpecifier { PUBLIC, PRIVATE, PROTECTED };
    struct ParentData {

        std::string name;
        bool isVirtual;
        AccessSpecifier accessSpecifier;

    };

    struct ClassData {

        std::string name;

        std::vector<ParentData> parents;

        std::vector<DeclTypePolicy> fields;
        std::vector<FunctionSignaturePolicy> methods;

    };

private:

    ClassData data;
    std::size_t classDepth;
    AccessSpecifier currentRegion;

public:


    ClassPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          classDepth(0),
          currentRegion(PUBLIC) { 
    
        InitializeClassPolicyHandlers();

    }

protected:
    void * DataInner() const override {

        return new ClassData(data);

    }

private:

    void InitializeClassPolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::classn] = [this](const srcSAXEventContext& ctx) {

            if(!classDepth) {

                classDepth = ctx.depth;
                data = ClassData{};
                CollectNameHandlers();
                CollectSuperHanders();
                CollectBlockHanders();

            }

        };

        // end of policy
        closeEventMap[ParserState::classn] = [this](const srcSAXEventContext& ctx) {

            if(classDepth && classDepth == ctx.depth) {

                classDepth = 0;
                NotifyAll();
                InitializeClassPolicyHandlers();

            }
           
        };

    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                closeEventMap[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx) { data.name += ctx.currentToken; };

            }

        };

        closeEventMap[ParserState::name] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::name});
                NopCloseEvents({ParserState::name, ParserState::tokenstring});

            }

        };

    }

    void CollectSuperHanders() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::super_list] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                openEventMap[ParserState::super] = [this](const srcSAXEventContext& ctx) {

                    data.parents.emplace_back(ParentData{ "", false, PUBLIC });

                };

                closeEventMap[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx) {

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

        closeEventMap[ParserState::super_list] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::super_list, ParserState::super});
                NopCloseEvents({ParserState::super_list, ParserState::tokenstring});

            }

        };

    }

    void CollectBlockHanders() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::block] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::name, ParserState::super_list, ParserState::super});
                NopCloseEvents({ParserState::name, ParserState::super_list, ParserState::tokenstring});

                // set up to listen to decl_stmt, member, and class policies

            }

        };

        // should always be in a region once block starts, so should not have to close
        openEventMap[ParserState::publicaccess] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 2) == ctx.depth) {

                currentRegion = PUBLIC;

            }

        };

        openEventMap[ParserState::protectedaccess] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 2) == ctx.depth) {

                currentRegion = PROTECTED;

            }

        };

        openEventMap[ParserState::privateaccess] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 2) == ctx.depth) {

                currentRegion = PRIVATE;

            }

        };

        openEventMap[ParserState::block] = [this](const srcSAXEventContext& ctx) {

            if((classDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::block, ParserState::publicaccess, ParserState::protectedaccess, ParserState::privateaccess});
                NopCloseEvents({ParserState::block});

            }

        };

    }

};
