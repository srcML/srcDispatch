// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffTypePolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#include <srcDiffExpressionPolicy.hpp>
#include <srcDiffTypePolicy.hpp>
#include <srcDispatchUtilities.hpp>
#include <DeltaElement.hpp>

namespace srcDiffDispatch {

    std::string TypeData::ToString(srcDispatch::DiffOperation operation) const {
        std::string str;
        bool printSpace = false;
        for (const std::pair<DeltaElement<std::any>, DeltaElement<TypeData::TypeType>>& type : types) {

            bool outputRaw = type.second.IsOfOperation(operation);
            if(outputRaw) {
                if(printSpace) {
                    str += ' ';
                }
                printSpace = true;
            }

            TypeData::TypeType outputsType = ((operation == srcDispatch::DELETE && type.second.HasOriginal()) || !type.second.HasModified())? type.second.GetOriginal() : type.second.GetModified();
            if(outputRaw && outputsType == TypeData::POINTER) {
                str += '*';
            } else if(outputRaw && outputsType == TypeData::REFERENCE) {
                str += '&';
            } else if(outputRaw && outputsType == TypeData::RVALUE) {
                str += "&&";
            } else if(outputsType == TypeData::SPECIFIER) {
                str += type.first.ToString<std::shared_ptr<std::string>>(operation);
            } else if(outputsType == TypeData::TYPENAME) {
                str += type.first.ToString<std::shared_ptr<NameData>>(operation);
            }

        }
        return str;
    }

    std::shared_ptr<TypeData> TypeData::copyAs(srcDispatch::DiffOperation operation) const {
        std::shared_ptr<TypeData> data = std::make_shared<TypeData>();
        data->lineNumber = lineNumber;
        for(const std::pair<DeltaElement<std::any>, DeltaElement<TypeType>>& type : types) {
            DeltaElement<std::any> typeAny;
            if(type.second.GetElement() == TypeData::SPECIFIER) {
                typeAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<std::string>>(type.first));
            } else if(type.second.GetElement() == TypeData::TYPENAME) {
                typeAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<NameData>>(type.first.GetElement())->copyAs(operation));
            }
            data->types.emplace_back(typeAny, DeltaElement(operation, type.second.GetElement()));
        }
        return data;
    }


    TypePolicy::TypePolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners), data{} {
        InitializeTypePolicyHandlers();
    }

    TypePolicy::~TypePolicy() {}

    void TypePolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
        data.types.push_back(std::make_pair(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<NameData>()), DeltaElement(ctx.diffStack.back().operation, TypeData::TYPENAME)));
        ctx.dispatcher->RemoveListenerDispatch(nullptr);
    }

    std::any TypePolicy::DataInner() const {
        return std::make_shared<TypeData>(data);
    }

    void TypePolicy::InitializeTypePolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if(!depth) {
                depth = ctx.depth;
                data = TypeData{};
                data.lineNumber = ctx.startLineNumber;
                CollectNamesHandler();
                CollectModifersHandler();
                CollectSpecifiersHandler();
            }
        };

        // end of policy
        closeEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeTypePolicyHandlers();
        };
    }

    void TypePolicy::CollectNamesHandler() {
        using namespace srcDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            if(!namePolicy) {
                namePolicy = make_unique_policy<NamePolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(namePolicy.get());
        };
    }

    void TypePolicy::CollectModifersHandler() {
        using namespace srcDispatch;
        openEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx)
        {
            if(!depth) return;

            data.types.push_back(std::make_pair(DeltaElement<std::any>(), DeltaElement(ctx.diffStack.back().operation, TypeData::NONE)));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                TypeData::TypeType modifier = TypeData::NONE;
                if(ctx.currentToken == "*") {
                    modifier = TypeData::POINTER;
                } else if(ctx.currentToken == "&") {
                    modifier = TypeData::REFERENCE;
                } else if(ctx.currentToken == "&&") {
                    modifier = TypeData::RVALUE;
                }

                data.types.back().second.Update(ctx.diffStack.back().operation, modifier);
            };
        };

        closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            NopCloseEvents({ParserState::tokenstring});
        };
    }

    void TypePolicy::CollectSpecifiersHandler() {
        using namespace srcDispatch;
        openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            data.types.push_back(std::make_pair(DeltaElement<std::any>(ctx.diffStack.back().operation, std::make_shared<std::string>("")), DeltaElement(ctx.diffStack.back().operation, TypeData::SPECIFIER)));
            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
                data.types.back().first.Append<std::shared_ptr<std::string>>(ctx.diffStack.back().operation, ctx.currentToken);
            };
        };

        closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {
            if(!depth) return;

            NopCloseEvents({ParserState::tokenstring});
        };
    }

}
