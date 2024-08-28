/**
 * @file TypePolicySingleEvent.cpp
 *
 */

#include <TypePolicySingleEvent.hpp>

std::string TypeData::ToString() const {
    std::string type_str;
    for (std::size_t pos = 0; pos < types.size(); ++pos) {
        if (pos != 0) type_str += ' ';
        const std::pair<void *, TypeType> & type = types[pos];
        if (type.second == TypeData::POINTER)
            type_str += '*';
        else if (type.second == TypeData::REFERENCE)
            type_str += '&';
        else if (type.second == TypeData::RVALUE)
            type_str += "&&";
        else if (type.second == TypeData::SPECIFIER)
            type_str += *static_cast<std::string *>(type.first);
        else if (type.second == TypeData::TYPENAME)
            type_str += static_cast<NameData *>(type.first)->ToString();
    }
    return type_str;
}

std::ostream & operator<<(std::ostream & out, const TypeData & typeData) {
    //std::cerr << "TPSE\n";
    //std::cerr << "TPSE Size: " << typeData.types.empty() << '\n';
    //std::cerr << "TACO\n";
    for(std::size_t pos = 0; pos < typeData.types.size(); ++pos) {
        //std::cerr << "TPSE2\n";
        if (pos != 0) out << ' ';
        const std::pair<void *, TypeData::TypeType> & type = typeData.types[pos];
        if (type.second == TypeData::POINTER)
            out << '*';
        else if (type.second == TypeData::REFERENCE)
            out << '&';
        else if (type.second == TypeData::RVALUE)
            out << "&&";
        else if (type.second == TypeData::SPECIFIER)
            out << *static_cast<std::string *>(type.first);
        else if (type.second == TypeData::TYPENAME)
            out << *static_cast<NameData *>(type.first);
    }
    return out;
}

TypePolicy::TypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
    : srcSAXEventDispatch::PolicyDispatcher(listeners),
      data{},
      typeDepth(0),
      namePolicy(nullptr) {

    InitializeTypePolicyHandlers();
}

TypePolicy::~TypePolicy(){
    if (namePolicy) delete namePolicy;
}

void TypePolicy::Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) {
    //this causes undefined behavior if types is empty
    data.types.back().first = policy->Data<NameData>();
    ctx.dispatcher->RemoveListenerDispatch(nullptr);
}

void TypePolicy::NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx){}

void * TypePolicy::DataInner() const {
    return new TypeData(data);
}

void TypePolicy::InitializeTypePolicyHandlers() {
    using namespace srcSAXEventDispatch;
    // start of policy
    openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
        if (!typeDepth) {
            typeDepth = ctx.depth;
            data = TypeData{};
            CollectNamesHandler();
            CollectModifersHandler();
            CollectSpecifiersHandler();
        }
    };

    // end of policy
    closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
        if (typeDepth && typeDepth == ctx.depth) {
            typeDepth = 0;
            NotifyAll(ctx);
            InitializeTypePolicyHandlers();
        }
    };
}

void TypePolicy::CollectNamesHandler() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if (typeDepth && (typeDepth + 1) == ctx.depth) {
            data.types.push_back(std::make_pair(nullptr, TypeData::TYPENAME));
            if (!namePolicy) namePolicy = new NamePolicy{this};
            ctx.dispatcher->AddListenerDispatch(namePolicy);
        }
    };
}

void TypePolicy::CollectModifersHandler() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
        if (typeDepth && (typeDepth + 1) == ctx.depth) {
            data.types.push_back(std::make_pair(nullptr, TypeData::NONE));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                if (ctx.currentToken == "*")
                    data.types.back().second = TypeData::POINTER;
                else if (ctx.currentToken == "&")
                    data.types.back().second = TypeData::REFERENCE;
                else if (ctx.currentToken == "&&")
                    data.types.back().second = TypeData::RVALUE;
            };
        }
    };

    closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
        if (typeDepth && (typeDepth + 1) == ctx.depth) {
            NopCloseEvents({ParserState::tokenstring});
        }
    };
}

void TypePolicy::CollectSpecifiersHandler() {
    using namespace srcSAXEventDispatch;
    openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
        if (typeDepth && (typeDepth + 1) == ctx.depth) {
            data.types.push_back(std::make_pair(new std::string(), TypeData::SPECIFIER));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                (*static_cast<std::string *>(data.types.back().first)) += ctx.currentToken;
            };
        }
    };

    closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
        if (typeDepth && (typeDepth + 1) == ctx.depth) {
            NopCloseEvents({ParserState::tokenstring});
        }
    };
}
