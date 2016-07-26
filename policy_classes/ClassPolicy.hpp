#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>
#include <TypePolicy.hpp>
#include <FunctionSignaturePolicy.hpp>

#include <string>
#include <vector>

class ClassPolicy : public srcSAXEventDispatch::EventListener {

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

        } data;

    private:

        std::size_t class_depth;

    public:

        ClassPolicy() : class_depth(0) { InitializeEventHandlers(); }

    private:

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            close_event_map[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx) {

                if(ctx.And({ ParserState::classn, ParserState::name }) && ctx.Nor({ ParserState::block })) {

                    data.name += ctx.currentToken;

                /** @todo wont work.  Nested/local classes always in block.  Need to use depth */
                } else if(ctx.And({ ParserState::classn, ParserState::super }) && ctx.Nor({ ParserState::block })) {

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

                }

            };

            // start of policy
            open_event_map[ParserState::classn] = [this](const srcSAXEventContext& ctx) {

                if(!class_depth)
                    class_depth = ctx.depth;

            };

            // end of policy
            close_event_map[ParserState::classn] = [this](const srcSAXEventContext& ctx) {

                if(class_depth == ctx.depth)
                    class_depth = 0;
               

            };

            open_event_map[ParserState::super] = [this](const srcSAXEventContext& ctx) {

                data.parents.emplace_back(ParentData{ "", false, PUBLIC });

            };

        }

};