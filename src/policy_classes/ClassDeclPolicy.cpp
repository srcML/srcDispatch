// // SPDX-License-Identifier: GPL-3.0-only
// /**
//  * @file ClassDeclPolicy.hpp
//  *
//  * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
//  *
//  * This file is part of the Dispatch Infrastructure.
//  */

// #include <ClassDeclPolicy.hpp>

// namespace srcDispatch {

// void ClassDeclPolicy::Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {
//     using namespace srcDispatch;

//     if(typeid(NamePolicy) == typeid(*policy)) {
//         data.name = DeltaElement(ctx.diffStack.back().operation, policy->Data<NameData>());
//     } else{
//         throw std::string("Unhandled ExpressionPolicy condition");
//     }

//     ctx.dispatcher->RemoveListenerDispatch(nullptr);
// }

// void ClassDeclPolicy::NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) {}

// std::any ClassDeclPolicy::DataInner() const { return std::make_shared<ClassDeclData>(data); }

// void ClassDeclPolicy::InitializeClassDeclPolicyHandlers() {
//     using namespace srcDispatch;
//     // start of policy
//     std::function<void(srcDispatch::srcSAXEventContext& )> startPolicy = [this](srcSAXEventContext& ctx) {
//         if(depth) return;

//         depth = ctx.depth;
//         data = ClassDeclData{};
//         data.namespaces = ctx.currentNamespaces;
//         data.startPosition = ctx.startPosition;
//         data.endPosition   = ctx.endPosition;
//         data.classDeclType = DeltaElement<ClassDeclData::ClassDeclType>(getType(ctx.dispatcher->CurrentPState()));
//         data.name = DeltaElement<std::shared_ptr<NameData>>();
//         data.language = ctx.currentFileLanguage;
//         data.filename = ctx.currentFilePath;

//         std::cerr << "hello again?\n";

//         CollectNameHandlers();
//     };

//     // end of policy
//     std::function<void(srcDispatch::srcSAXEventContext&)> endPolicy = [this](srcSAXEventContext& ctx) {
//         if(!depth || depth != ctx.depth) return;

//         depth = 0;
//         NotifyAll(ctx);
//         InitializeClassDeclPolicyHandlers();
//     };

//     openEventMap[ParserState::classdecl] = startPolicy;
//     closeEventMap[ParserState::classdecl] = endPolicy;

//     openEventMap[ParserState::enumdecl] = startPolicy;
//     closeEventMap[ParserState::enumdecl] = endPolicy;

//     openEventMap[ParserState::structdecl] = startPolicy;
//     closeEventMap[ParserState::structdecl] = endPolicy;

//     openEventMap[ParserState::uniondecl] = startPolicy;
//     closeEventMap[ParserState::uniondecl] = endPolicy;
// }

// void ClassDeclPolicy::CollectNameHandlers() {
//     using namespace srcDispatch;
//     openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
//         if(!depth) return;

//         if(!namePolicy) {
//             namePolicy = make_unique_policy<NamePolicy>({this});
//         }
//         ctx.dispatcher->AddListenerDispatch(namePolicy.get());
//     };
//     closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx [[maybe_unused]]) {
//         if(!depth) return;

//         NopOpenEvents({ParserState::name});
//         NopCloseEvents({ParserState::name});
//     };
// }
// }
