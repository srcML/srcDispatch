// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ClassDeclPolicy.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_CLASS_DECL_POLICY_HPP
#define INCLUDED_CLASS_DECL_POLICY_HPP

#include <srcDispatchUtilities.hpp>
#include <Access.hpp>

#include <ElementData.hpp>
#include <DeltaElement.hpp>

#include <NamePolicy.hpp>

namespace srcDispatch {
    
    struct ClassDeclData: public ElementData {
        enum ClassDeclType: std::size_t { CLASS, ENUM, STRUCT, UNION };

        DeltaElement<std::shared_ptr<NameData>> name;
        DeltaElement<ClassDeclType> classDeclType;
        
        std::vector<std::string> namespaces;

        std::string language;
        std::string filename;

        template<class type>
        friend class DeltaElement;
    private:
        std::string ToString(srcDispatch::DiffOperation operation) const {
            std::string str;

            switch (this->classDeclType.GetOfOperation(operation)) {
                case ClassDeclType::CLASS: str += "class"; break;
                case ClassDeclType::ENUM: str += "enum"; break;
                case ClassDeclType::STRUCT: str += "struct"; break;
                case ClassDeclType::UNION: str += "union"; break;
            }

            if (name) {
                str += " " + name.ToString(operation);
            }

            return str;
        }

    };

    class ClassDeclPolicy:     
    public srcDispatch::EventListener,
    public srcDispatch::PolicyDispatcher,
    public srcDispatch::PolicyListener {

    private:
        ClassDeclData data;

        std::unique_ptr<NamePolicy> namePolicy;

        static ClassDeclData::ClassDeclType getType(srcDispatch::ParserState pstate) {
            switch (pstate) {
                case srcDispatch::classdecl: return ClassDeclData::ClassDeclType::CLASS;
                case srcDispatch::enumdecl: return ClassDeclData::ClassDeclType::ENUM;
                case srcDispatch::structdecl: return ClassDeclData::ClassDeclType::STRUCT;
                case srcDispatch::uniondecl: return ClassDeclData::ClassDeclType::UNION;
                default: throw std::string("Unhandled Class Declaration Type");
            }
        }

    public:
        ClassDeclPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners), data{} {
            std::cout << "in ClassDeclPolicy constructor\n";
            InitializeClassDeclPolicyHandlers();
        }

        ~ClassDeclPolicy() {}

    protected:
 void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
    using namespace srcDispatch;

    if(typeid(NamePolicy) == typeid(*policy)) {
        data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
    } else{
        throw std::string("Unhandled ExpressionPolicy condition");
    }

    ctx.dispatcher->RemoveListenerDispatch(nullptr);
}

void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) {}

std::any DataInner() const { return std::make_shared<ClassDeclData>(data); }
private:
void InitializeClassDeclPolicyHandlers() {
    using namespace srcDispatch;
    // start of policy
    std::function<void(srcDispatch::srcSAXEventContext& )> startPolicy = [this](srcSAXEventContext& ctx) {
        if(depth) return;

        depth = ctx.depth;
        data = ClassDeclData{};
        data.namespaces = ctx.currentNamespaces;
        data.startPosition = ctx.startPosition;
        data.endPosition   = ctx.endPosition;
        data.classDeclType = DeltaElement<ClassDeclData::ClassDeclType>(getType(ctx.dispatcher->CurrentPState()));
        data.name = DeltaElement<std::shared_ptr<NameData>>();
        data.language = ctx.currentFileLanguage;
        data.filename = ctx.currentFilePath;

        std::cerr << "hello again?\n";

        CollectNameHandlers();
    };

    // end of policy
    std::function<void(srcDispatch::srcSAXEventContext&)> endPolicy = [this](srcSAXEventContext& ctx) {
        if(!depth || depth != ctx.depth) return;

        depth = 0;
        NotifyAll(ctx);
        InitializeClassDeclPolicyHandlers();
    };

    openEventMap[ParserState::classdecl] = startPolicy;
    closeEventMap[ParserState::classdecl] = endPolicy;

    openEventMap[ParserState::enumdecl] = startPolicy;
    closeEventMap[ParserState::enumdecl] = endPolicy;

    openEventMap[ParserState::structdecl] = startPolicy;
    closeEventMap[ParserState::structdecl] = endPolicy;

    openEventMap[ParserState::uniondecl] = startPolicy;
    closeEventMap[ParserState::uniondecl] = endPolicy;
}

void CollectNameHandlers() {
    using namespace srcDispatch;
    openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
        if(!depth) return;

        if(!namePolicy) {
            namePolicy = make_unique_policy<NamePolicy>({this});
        }
        ctx.dispatcher->AddListenerDispatch(namePolicy.get());
    };
    closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx [[maybe_unused]]) {
        if(!depth) return;

        NopOpenEvents({ParserState::name});
        NopCloseEvents({ParserState::name});
    };
}
    };
}

#endif