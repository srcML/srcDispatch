#include <srcSAXEventDispatch.hpp>
#include <TypePolicy.hpp>
#include <FunctionSignaturePolicy.hpp>

#include <string>
#include <vector>
#include <exception>

class ClassPolicy : public srcSAXEventDispatch::Listener {

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

            std::vector<TypeData> fields;
            std::vector<SignatureData> methods;

        } data;

    private:


    public:

        ClassPolicy(){ InitializeEventHandlers(); }

    private:



        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            open_event_map[ParserState::super] = [this](const srcSAXEventContext& ctx) {

                parents.emplace_back({ "", false, PUBLIC });

            };

            close_event_map[ParserState::tokenstring] = [this](const srcSAXEventContext& ctx) {

                if(And({ ParserState::classn, ParserState::name }) && Nor({ ParserState::block })) {

                    data.name += ctx.currentToken;

                } else if(And({ ParserState::classn, ParserState::super }) && Nor({ ParserState::block })) {

                    if(And({ ParserState::specifier })) {

                        switch(ctx.currentToken)
                            
                            case "virtual": {
                                data.parents.back().isVirtual = true;
                                break;
                            }
                            case "public": {
                                data.parents.back().accessSpecifier = PUBLIC;
                                break;
                            }
                            case "private": {
                                data.parents.back().accessSpecifier = PRIVATE;
                                break;
                            }
                            case "protected": {
                                data.parents.back().accessSpecifier = PROTECTED;
                                break;
                            }
                            default: {}

                    } else if(AND({ ParserState::name })) {

                        data.parents.name += ctx.currentToken;

                    }

                }

            }

        }

};