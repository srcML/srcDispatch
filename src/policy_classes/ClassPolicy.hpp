/**
 * @file ClassPolicy.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_CLASS_POLICY_HPP
#define INCLUDED_CLASS_POLICY_HPP

#include <srcSAXEventDispatcher.hpp>
#include <DeclTypePolicy.hpp>
#include <DeclDS.hpp>
#include <FunctionSignaturePolicy.hpp>
#include <srcSAXHandler.hpp>
#include <exception>
#include <stack>
#include <list>

class ClassPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {
    public:

    	struct ClassData {
    		void Clear() {
      			className = "";
      			methods.clear();
      			members.clear();
      			isStruct = false;
   			}

   			std::string className;
   			bool isStruct = false;  //False -> Class; True -> Struct
   			std::vector<SignatureData> methods;
   			std::vector<DeclData> members;
		};

        ~ClassPolicy() {
            delete funcSigPolicy;
            delete declTypePolicy;
        }

        ClassPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}): srcSAXEventDispatch::PolicyDispatcher(listeners) {
            funcSigPolicy = new FunctionSignaturePolicy({this});
            declTypePolicy = new DeclTypePolicy({this});

            InitializeEventHandlers();
        }

        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            if (typeid(FunctionSignaturePolicy) == typeid(*policy)) {
                SignatureData signatureData = *policy->Data<SignatureData>();
                data_stack.top().methods.push_back(signatureData);
            }

            else if (typeid(DeclTypePolicy) == typeid(*policy)) {
                if(!(ctx.IsOpen(srcSAXEventDispatch::ParserState::function))) {
                    DeclData declarationData = *policy->Data<DeclData>();
                    data_stack.top().members.push_back(declarationData);
                }
            }
        }

    protected:

        void * DataInner() const override {
            return new std::vector<ClassData>(data);
        }

    private:

        std::stack<ClassData> data_stack;

        std::vector<ClassData> data;

        FunctionSignaturePolicy *funcSigPolicy;
        DeclTypePolicy *declTypePolicy;

        bool gotClassName = true;

        void InitializeEventHandlers() {
            using namespace srcSAXEventDispatch;

            //Classes
            openEventMap[ParserState::classn] = [this](srcSAXEventContext &ctx) {
                if(data_stack.empty()) {
                    ctx.dispatcher->AddListenerDispatch(funcSigPolicy);
                    ctx.dispatcher->AddListenerDispatch(declTypePolicy);
                }

                ClassData current; 
                current.isStruct = false;
                data_stack.push(current);

                gotClassName = false;
            };

            closeEventMap[ParserState::classn] = [this](srcSAXEventContext &ctx) {
                data.push_back(data_stack.top());
                data_stack.pop();

                if(data_stack.empty()) {
                    ctx.dispatcher->RemoveListenerDispatch(funcSigPolicy);
                    ctx.dispatcher->RemoveListenerDispatch(declTypePolicy);

                    NotifyAll(ctx);
                    data.clear();
                }
            };

            //Structs
            openEventMap[ParserState::structn] = [this](srcSAXEventContext &ctx) {
                if(data_stack.empty()) {
                    ctx.dispatcher->AddListenerDispatch(funcSigPolicy);
                    ctx.dispatcher->AddListenerDispatch(declTypePolicy);
                }

                ClassData current; 
                current.isStruct = true;
                data_stack.push(current);

                gotClassName = false;
            };

            closeEventMap[ParserState::structn] = [this](srcSAXEventContext &ctx) {
                data.push_back(data_stack.top());
                data_stack.pop();

                if(data_stack.empty()) {
                    ctx.dispatcher->RemoveListenerDispatch(funcSigPolicy);
                    ctx.dispatcher->RemoveListenerDispatch(declTypePolicy);

                    NotifyAll(ctx);
                    data.clear();
                }
            };

            closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx){
                if((ctx.IsOpen(ParserState::structn) || ctx.IsOpen(ParserState::classn)) && !gotClassName) {
                    data_stack.top().className = ctx.currentToken;
                    gotClassName = true;
                }
            };


        }
};

#endif