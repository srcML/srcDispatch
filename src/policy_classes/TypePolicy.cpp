#include <TypePolicy.hpp>

#include <NamePolicy.hpp>

std::ostream & operator<<(std::ostream & out, const TypePolicy::TypeData & typeData) {

    for(std::size_t pos = 0; pos < typeData.types.size(); ++pos) {

        if(pos != 0) out << ' ';

        const std::pair<void *, TypePolicy::TypeType> & type = typeData.types[pos];

        if(type.second == TypePolicy::POINTER)
            out << '*';
        else if(type.second == TypePolicy::REFERENCE)
            out << '&';
        else if(type.second == TypePolicy::RVALUE)
            out << "&&";
        else if(type.second == TypePolicy::SPECIFIER)
            out << *static_cast<std::string *>(type.first);
        else if(type.second == TypePolicy::NAME)
            out << *static_cast<NamePolicy::NameData *>(type.first);

    }

    return out;

}

TypePolicy::TypePolicy(SingleEventPolicyDispatcher & policy_handler, std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
      policy_handler(policy_handler),
      data{},
      typeDepth(0),
      namePolicy(nullptr) {

    InitializeTypePolicyHandlers();

}

TypePolicy::~TypePolicy(){

    if(namePolicy) delete namePolicy;

}

void TypePolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {

    data.types.back().first = policy->Data<NamePolicy::NameData>();
    policy_handler.PopListenerDispatch();

}

void * TypePolicy::DataInner() const {
    return new TypePolicy::TypeData(data);
}

void TypePolicy::InitializeTypePolicyHandlers() {
    using namespace srcSAXEventDispatch;

    // start of policy
    openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

        if(!typeDepth) {

            typeDepth = ctx.depth;
            data = TypePolicy::TypeData{};

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

void TypePolicy::CollectNamesHandler() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

        if(typeDepth && (typeDepth + 1) == ctx.depth) {

            data.types.push_back(std::make_pair(nullptr, TypePolicy::NAME));
            if(!namePolicy) namePolicy = new NamePolicy(policy_handler, {this});
            policy_handler.PushListenerDispatch(namePolicy);

        }

    };

}

void TypePolicy::CollectModifersHandler() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

        if(typeDepth && (typeDepth + 1) == ctx.depth) {

            data.types.push_back(std::make_pair(nullptr, NONE));

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                if(ctx.currentToken == "*")
                    data.types.back().second = TypePolicy::POINTER;
                else if(ctx.currentToken == "&")
                    data.types.back().second = TypePolicy::REFERENCE;
                else if(ctx.currentToken == "&&")
                    data.types.back().second = TypePolicy::RVALUE;

            };

        }

    };

    closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {

        if(typeDepth && (typeDepth + 1) == ctx.depth) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

}

void TypePolicy::CollectSpecifiersHandler() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

        if(typeDepth && (typeDepth + 1) == ctx.depth) {

            data.types.push_back(std::make_pair(new std::string(), TypePolicy::SPECIFIER));

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
