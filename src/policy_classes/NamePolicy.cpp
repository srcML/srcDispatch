// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffNamePolicy.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#include <NamePolicy.hpp>
#include <DeltaElement.hpp>
#include <ExpressionPolicy.hpp>

#include <cassert>

namespace srcDiffDispatch {

    std::string NameData::SimpleName() const {
        if(name) return name.ToString();

        // Not sure how safe GetElement use is
        assert(names.back().GetElement().type() == typeid(std::shared_ptr<NameData>));
        return names.back().ToString<std::shared_ptr<NameData>>();
    }

    std::string NameData::ToString(srcDispatch::DiffOperation operation) const {

        std::string str = name.ToString(operation);
        for (const DeltaElement<std::any>& name_element : names) {
            if(name_element.GetElement().type() == typeid(std::shared_ptr<NameData>)) {
                str += name_element.ToString<std::shared_ptr<NameData>>(operation);
            } else {
                str += name_element.ToString<std::shared_ptr<OperatorData>>(operation);
            }
        }
        if(templateArgumentList) {
            str += templateArgumentList.ToString(operation);
        }
        for (const DeltaElement<std::shared_ptr<ExpressionData>>& index : indices) {

            bool outputRaw = index.IsOfOperation(operation);
            if(outputRaw) {
                str += '[';
            }

            str += index.ToString(operation);

            if(outputRaw) {
                str += ']';
            }

        }

        return str;
    }

    std::shared_ptr<NameData> NameData::copyAs(srcDispatch::DiffOperation operation) const {
        std::shared_ptr<NameData> data = std::make_shared<NameData>();
        data->lineNumber = lineNumber;
        if(name) {
            data->name = DeltaElement<std::string>(operation, name.GetElement());
        }

        for (const DeltaElement<std::any>& name_element : names) {
            DeltaElement<std::any> nameAny;
            if(name_element.GetElement().type() == typeid(std::shared_ptr<NameData>)) {
                nameAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<NameData>>(name_element)->copyAs(operation));
            } else {
                nameAny = DeltaElement<std::any>(operation, std::any_cast<std::shared_ptr<OperatorData>>(name_element)->copyAs(operation));
            }
            data->names.emplace_back(nameAny);
        }

        if(templateArgumentList) {
            data->templateArgumentList = DeltaElement(operation, templateArgumentList.GetElement()->copyAs(operation)); 
        }

        for (const DeltaElement<std::shared_ptr<ExpressionData>>& index : indices) {
            data->indices.emplace_back(DeltaElement(operation, index.GetElement()->copyAs(operation)));
        }

        return data;
    }

    NamePolicy::~NamePolicy() {}

    void NamePolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {

        if(typeid(NamePolicy) == typeid(*policy)) {
            data.names.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<NameData>()));
        } else if(typeid(OperatorPolicy) == typeid(*policy)) {
            data.names.push_back(DeltaElement<std::any>(ctx.diffStack.back().operation, policy->Data<OperatorData>()));
        } else if(typeid(GenericArgumentsPolicy) == typeid(*policy)) {
            data.templateArgumentList = DeltaElement(ctx.diffStack.back().operation, policy->Data<GenericArgumentsData>());
        } else if(typeid(ExpressionPolicy) == typeid(*policy)) {
            data.indices.push_back(DeltaElement(ctx.diffStack.back().operation, policy->Data<ExpressionData>()));
        } else {
            throw srcDispatch::PolicyError(std::string("Unhandled Policy '") + typeid(*policy).name() + '\'');
        }
        ctx.dispatcher->RemoveListener(nullptr);
    }

    void NamePolicy::InitializeNamePolicyHandlers() {
        using namespace srcDispatch;

        // start of policy
        openEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
            if(!depth) {
                depth = ctx.depth;
                data = NameData{};
                data.lineNumber = ctx.startLineNumber;
                CollectOperatorsHandlers();
                CollectGenericArgumentsHandlers();
                CollectArrayIndicesHandlers();
            } else {
                NopCloseEvents({ParserState::tokenstring});
                if(!namePolicy) {
                    namePolicy = make_unique_policy<NamePolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(namePolicy.get());
            }
        };

        // end of policy
        closeEventMap[ParserState::name] = [this](srcSAXEventContext &ctx) {
            if(!depth || depth != ctx.depth) return;

            depth = 0;
            NotifyAll(ctx);
            InitializeNamePolicyHandlers();
        };

        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext &ctx) {
            if(!depth) return;

            if(data.name.GetOperation() == srcDispatch::NONE) {
                bool isChange = (ctx.depth + 1) == ctx.diffStack.back().depth && ctx.diffStack.back().isReplace;
                srcDispatch::DiffOperation operation = isChange? CHANGE : ctx.diffStack.back().operation;
                data.name = DeltaElement<std::string>(operation);
            }

            if(   ctx.diffStack.back().operation == srcDispatch::COMMON
                || ctx.diffStack.back().operation == srcDispatch::DELETE) {
                data.name.GetOriginal() += ctx.currentToken;
            } else if(ctx.diffStack.back().operation == srcDispatch::INSERT) {
                data.name.GetModified() += ctx.currentToken;
            }
        };
    }

    void NamePolicy::CollectOperatorsHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::op] = [this](srcSAXEventContext &ctx) {
            if(!depth) return;

            if(!operatorPolicy) {
                operatorPolicy = make_unique_policy<OperatorPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(operatorPolicy.get());
        };
    }

    void NamePolicy::CollectGenericArgumentsHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::genericargumentlist] = [this](srcSAXEventContext &ctx) {
            if(!depth) return;

            if(!templateArgumentListPolicy) {
                templateArgumentListPolicy = make_unique_policy<GenericArgumentsPolicy>({this});
            }
            ctx.dispatcher->AddListenerDispatch(templateArgumentListPolicy.get());
        };
    }

    void NamePolicy::CollectArrayIndicesHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::index] = [this](srcSAXEventContext &ctx) {
            if(!depth) return;

            openEventMap[ParserState::expr] = [this](srcSAXEventContext &ctx) {
                if(!expressionPolicy) {
                    expressionPolicy = make_unique_policy<ExpressionPolicy>({this});
                }
                ctx.dispatcher->AddListenerDispatch(expressionPolicy.get());
            };
        };
    }

}
