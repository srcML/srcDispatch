#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>

#include <exception>

#ifndef INCLUDED_TYPE_POLICY_HPP
#define INCLUDED_TYPE_POLICY_HPP

class NamePolicy;
class TypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:
    enum TypeType { NAME, POINTER, REFERENCE, RVALUE, SPECIFIER, NONE };

    struct TypeData {
        std::vector<std::pair<void *, TypeType>> types;

        friend std::ostream & operator<<(std::ostream & out, const TypeData & typeData) {

            for(std::size_t pos = 0; pos < typeData.types.size(); ++pos) {

                if(pos != 0) out << ' ';

                const std::pair<void *, TypeType> & type = typeData.types[pos];

                if(type.second == POINTER)
                    out << '*';
                else if(type.second == REFERENCE)
                    out << '&';
                else if(type.second == RVALUE)
                    out << "&&";
                else if(type.second == SPECIFIER)
                    out << *static_cast<std::string *>(type.first);

            }

            return out;

        }

    };
    private:
        TypeData data;
        std::size_t typeDepth;

    public:
        TypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {})
            : srcSAXEventDispatch::PolicyDispatcher(listeners),
              data{},
              typeDepth(0) {

            InitializeTypePolicyHandlers();

        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
    
            // data.name = policy->Data<NamePolicy::NameData>();
    
        }
    protected:
        void * DataInner() const override {
            return new TypeData(data);
        }
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;
        void InitializeTypePolicyHandlers() {
            using namespace srcSAXEventDispatch;

            // start of policy
            openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

                if(!typeDepth) {

                    typeDepth = ctx.depth;
                    data = TypeData{};

                    CollectNamesHandler();
                    CollectModifersHandler();
                    CollectSpecifiersHandler();

                }

            };

            // end of policy
            closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

                if(typeDepth && typeDepth == ctx.depth) {

                    typeDepth = 0;

                    NotifyAll(ctx);
                    InitializeTypePolicyHandlers();

                }
               
            };

        }

    void CollectNamesHandler() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(typeDepth && (typeDepth + 1) == ctx.depth) {

            }

        };

        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(typeDepth && (typeDepth + 1) == ctx.depth) {

            }

        };

    }

    void CollectModifersHandler() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

            if(typeDepth && (typeDepth + 1) == ctx.depth) {

                data.types.push_back(std::make_pair(nullptr, NONE));

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                    if(ctx.currentToken == "*")
                        data.types.back().second = POINTER;
                    else if(ctx.currentToken == "&")
                        data.types.back().second = REFERENCE;
                    else if(ctx.currentToken == "&&")
                        data.types.back().second = RVALUE;

                };

            }

        };

        closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

            if(typeDepth && (typeDepth + 1) == ctx.depth) {

                NopCloseEvents({ParserState::tokenstring});

            }

        };

    }

    void CollectSpecifiersHandler() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

            if(typeDepth && (typeDepth + 1) == ctx.depth) {

                data.types.push_back(std::make_pair(new std::string(), SPECIFIER));

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                    (*static_cast<std::string *>(data.types.back().first)) += ctx.currentToken;

                };

            }

        };

        closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

            if(typeDepth && (typeDepth + 1) == ctx.depth) {

                NopCloseEvents({ParserState::tokenstring});

            }

        };

    }

};

#endif
