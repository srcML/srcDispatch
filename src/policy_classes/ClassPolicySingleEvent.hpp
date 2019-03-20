/**
 * @file ClassPolicySingleEvent.hpp
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
#include <srcSAXEventDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
#include <DeclTypePolicySingleEvent.hpp>
#include <FunctionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

#ifndef INCLUDED_CLASS_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CLASS_POLICY_SINGLE_EVENT_HPP

class ClassPolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

	enum ClassType : std::size_t { CLASS, STRUCT/*, UNION, ENUM*/ };
	enum AccessSpecifier { PUBLIC = 0, PRIVATE = 1, PROTECTED = 2 };
	struct ParentData {

		// should this be a NamePolicy::NameData?
		std::string name;
		bool isVirtual;
		AccessSpecifier accessSpecifier;

	};

	struct ClassData {

		ClassType type;
		std::set<std::string> stereotypes;

		bool isGeneric;
		NamePolicy::NameData * name;

		std::vector<ParentData> parents;

		std::vector<DeclTypePolicy::DeclTypeData *> fields[3];
		std::vector<FunctionPolicy::FunctionData *> constructors[3];
		bool hasDestructor;
		std::vector<FunctionPolicy::FunctionData *> operators[3];
		std::vector<FunctionPolicy::FunctionData *> methods[3];

		std::vector<ClassPolicy::ClassData *> innerClasses[3];

		bool hasPureVirtual;

	};

private:

	ClassData data;
	std::size_t classDepth;
	AccessSpecifier currentRegion;

	NamePolicy * namePolicy;
	DeclTypePolicy * declPolicy;
	FunctionPolicy * functionPolicy;
	ClassPolicy * classPolicy;

public:

	ClassPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
		: srcSAXEventDispatch::PolicyDispatcher(listeners),
		  data{},
		  classDepth(0),
		  currentRegion(PUBLIC),
		  namePolicy(nullptr),
		  declPolicy(nullptr),
		  functionPolicy(nullptr),
		  classPolicy(nullptr) { 
	
		InitializeClassPolicyHandlers();

	}

	~ClassPolicy() {

		if(namePolicy)     delete namePolicy;
		if(declPolicy)     delete declPolicy;
		if(functionPolicy) delete functionPolicy;
		if(classPolicy)    delete classPolicy;

	}

	void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
	void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

		if(typeid(NamePolicy) == typeid(*policy)) {

			data.name = policy->Data<NamePolicy::NameData>();
			ctx.dispatcher->RemoveListenerDispatch(nullptr);

		} else if(typeid(DeclTypePolicy) == typeid(*policy)) {
	
			std::vector<DeclTypePolicy::DeclTypeData *> * decl_data = policy->Data<std::vector<DeclTypePolicy::DeclTypeData *>>();
			for(DeclTypePolicy::DeclTypeData * decl : *decl_data)
				data.fields[currentRegion].emplace_back(decl);
			decl_data->clear();
			ctx.dispatcher->RemoveListenerDispatch(nullptr);

		} else if(typeid(FunctionPolicy) == typeid(*policy)) {

			FunctionPolicy::FunctionData * f_data = policy->Data<FunctionPolicy::FunctionData>();

			if(f_data->isPureVirtual)
				data.hasPureVirtual = true;

			if(f_data->type == FunctionPolicy::CONSTRUCTOR)
				data.constructors[currentRegion].emplace_back(f_data);
			else if(f_data->type == FunctionPolicy::OPERATOR)
				data.operators[currentRegion].emplace_back(f_data);
			else 
				data.methods[currentRegion].emplace_back(f_data);
			ctx.dispatcher->RemoveListenerDispatch(nullptr);

		} else if(typeid(ClassPolicy) == typeid(*policy)) {

			data.innerClasses[currentRegion].emplace_back(policy->Data<ClassPolicy::ClassData>());
			ctx.dispatcher->RemoveListener(nullptr);

		}

	}

protected:
	void * DataInner() const override {

		return new ClassData(data);

	}

private:

	void InitializeClassPolicyHandlers() {
		using namespace srcSAXEventDispatch;

		// start of policy
		std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> startPolicy = [this](srcSAXEventContext& ctx) {

			if(!classDepth) {

				classDepth = ctx.depth;

				data = ClassData{};
				std::map<std::string, std::string>::const_iterator stereotype_attr_itr = ctx.attributes.find("stereotype");

				if(stereotype_attr_itr != ctx.attributes.end()){
					std::istringstream stereostring(stereotype_attr_itr->second);
					data.stereotypes = std::set<std::string>(std::istream_iterator<std::string>(stereostring), std::istream_iterator<std::string>());
				}

				if(ctx.currentTag == "class")
					data.type = CLASS;
				else if(ctx.currentTag == "struct")
					data.type = STRUCT;

				data.name = nullptr;

				CollectNameHandlers();
				CollectGenericHandlers();
				CollectSuperHanders();
				CollectBlockHanders();

			} else if((classDepth + 3) == ctx.depth) {

				if(!classPolicy) classPolicy = new ClassPolicy{this};
				ctx.dispatcher->AddListenerDispatch(classPolicy);

			}

		};

		// end of policy
		std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> endPolicy = [this](srcSAXEventContext& ctx) {

			if(classDepth && classDepth == ctx.depth) {

				classDepth = 0;
				NotifyAll(ctx);
				InitializeClassPolicyHandlers();

			}
		   
		};

		openEventMap[ParserState::classn] = startPolicy;
		closeEventMap[ParserState::classn] = endPolicy;
		openEventMap[ParserState::structn] = startPolicy;
		closeEventMap[ParserState::structn] = endPolicy;

	}

	void CollectNameHandlers() {
		using namespace srcSAXEventDispatch;

		openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				if(!namePolicy) namePolicy = new NamePolicy{this};
				ctx.dispatcher->AddListenerDispatch(namePolicy);

			}

		};

		closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				NopOpenEvents({ParserState::name});
				NopCloseEvents({ParserState::name});

			}

		};

	}

	void CollectGenericHandlers() {
		using namespace srcSAXEventDispatch;

		closeEventMap[ParserState::templates] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				data.isGeneric = true;

			}

		};

	}

	void CollectSuperHanders() {
		using namespace srcSAXEventDispatch;

		openEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				openEventMap[ParserState::super] = [this](srcSAXEventContext& ctx) {

					data.parents.emplace_back(ParentData{ "", false, PUBLIC });

				};

				closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

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


				};              

			}

		};

		closeEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				NopOpenEvents({ParserState::super_list, ParserState::super});
				NopCloseEvents({ParserState::super_list, ParserState::tokenstring});

			}

		};

	}

	void CollectBlockHanders() {
		using namespace srcSAXEventDispatch;

	   openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				NopOpenEvents({ParserState::name, ParserState::super_list, ParserState::super});
				NopCloseEvents({ParserState::name, ParserState::super_list, ParserState::tokenstring});

				// set up to listen to decl_stmt, member, and class policies
				openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

					if((classDepth + 3) == ctx.depth) {

						if(!declPolicy) declPolicy = new DeclTypePolicy{this};
						ctx.dispatcher->AddListenerDispatch(declPolicy);

					}

				};
				std::function<void (srcSAXEventContext& ctx)> functionEvent = [this](srcSAXEventContext& ctx) {

					if((classDepth + 3) == ctx.depth) {

						if(!functionPolicy) functionPolicy = new FunctionPolicy{this};
						ctx.dispatcher->AddListenerDispatch(functionPolicy);

					}

				};
				openEventMap[ParserState::function] = functionEvent;
				openEventMap[ParserState::functiondecl] = functionEvent;
				openEventMap[ParserState::constructor] = functionEvent;
				openEventMap[ParserState::constructordecl] = functionEvent;

				std::function<void (srcSAXEventContext& ctx)> destructorEvent = [this](srcSAXEventContext& ctx) {

					if((classDepth + 3) == ctx.depth) {

						data.hasDestructor = true;

					}

				};

				openEventMap[ParserState::destructor] = destructorEvent;
				openEventMap[ParserState::destructordecl] = destructorEvent;

			}

		};


		// should always be in a region once block starts, so should not have to close
		openEventMap[ParserState::publicaccess] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 2) == ctx.depth) {

				currentRegion = PUBLIC;

			}

		};

		openEventMap[ParserState::protectedaccess] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 2) == ctx.depth) {

				currentRegion = PROTECTED;

			}

		};

		openEventMap[ParserState::privateaccess] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 2) == ctx.depth) {

				currentRegion = PRIVATE;

			}

		};

		closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {

			if((classDepth + 1) == ctx.depth) {

				NopOpenEvents({ParserState::block, ParserState::function, ParserState::functiondecl, 
							   ParserState::constructor, ParserState::constructordecl, ParserState::destructor, ParserState::destructordecl,
							   ParserState::declstmt,
							   ParserState::publicaccess, ParserState::protectedaccess, ParserState::privateaccess});
				NopCloseEvents({ParserState::block});

			}

		};

	}

};

#endif
