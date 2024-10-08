/**
 * @file ClassPolicySingleEvent.hpp
 *
 */
#ifndef INCLUDED_CLASS_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_CLASS_POLICY_SINGLE_EVENT_HPP

#include <srcSAXEventDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
#include <DeclTypePolicySingleEvent.hpp>
#include <FunctionPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>


struct ParentData;

struct ClassData {
    enum ClassType : std::size_t { CLASS, STRUCT };  //UNION, ENUM?
    enum AccessSpecifier         { PUBLIC = 0, PRIVATE = 1, PROTECTED = 2 };

    unsigned int lineNumber;
    std::string               language;
    std::string               filename;
    ClassType                 type;
    std::set<std::string>     stereotypes;
    std::shared_ptr<NameData> name;
    std::vector<ParentData>   parents;

    std::vector<std::shared_ptr<DeclTypeData>> fields[3];
    std::vector<std::shared_ptr<FunctionData>> constructors[3];
    std::vector<std::shared_ptr<FunctionData>> operators[3];
    std::vector<std::shared_ptr<FunctionData>> methods[3];
    std::vector<std::shared_ptr<ClassData>>    innerClasses[3];

    bool hasDestructor;
    bool isGeneric;
    bool hasPureVirtual;
};

struct ParentData {
    std::string name;
    bool isVirtual;
    ClassData::AccessSpecifier accessSpecifier;
};


class ClassPolicy :
public srcSAXEventDispatch::EventListener,
public srcSAXEventDispatch::PolicyDispatcher,
public srcSAXEventDispatch::PolicyListener {

private:
	ClassData                  data;
	std::size_t                classDepth;
    ClassData::AccessSpecifier currentRegion;

	NamePolicy     * namePolicy;
	DeclTypePolicy * declPolicy;
	FunctionPolicy * functionPolicy;
	ClassPolicy    * classPolicy;

public:
	ClassPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
		: srcSAXEventDispatch::PolicyDispatcher(listeners),
		  data{},
		  classDepth(0),
		  currentRegion(ClassData::PUBLIC),
		  namePolicy(nullptr),
		  declPolicy(nullptr),
		  functionPolicy(nullptr),
		  classPolicy(nullptr) {
		InitializeClassPolicyHandlers();
	}

	~ClassPolicy() {
		if (namePolicy)     delete namePolicy;
		if (declPolicy)     delete declPolicy;
		if (functionPolicy) delete functionPolicy;
		if (classPolicy)    delete classPolicy;
	}

	void NotifyWrite(const PolicyDispatcher * policy [[maybe_unused]], srcSAXEventDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} //doesn't use other parsers

	void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
		if (typeid(NamePolicy) == typeid(*policy)) {
			data.name = policy->Data<NameData>();
			ctx.dispatcher->RemoveListenerDispatch(nullptr);
		} else if (typeid(DeclTypePolicy) == typeid(*policy)) {
			std::shared_ptr<std::vector<std::shared_ptr<DeclTypeData>>> decl_data = policy->Data<std::vector<std::shared_ptr<DeclTypeData>>>();
			for (std::shared_ptr<DeclTypeData> decl : *decl_data)
				data.fields[currentRegion].emplace_back(decl);
			decl_data->clear();
			ctx.dispatcher->RemoveListenerDispatch(nullptr);
		} else if (typeid(FunctionPolicy) == typeid(*policy)) {
			std::shared_ptr<FunctionData> f_data = policy->Data<FunctionData>();
			if (f_data->isPureVirtual)
				data.hasPureVirtual = true;
			if (f_data->type == FunctionData::CONSTRUCTOR)
				data.constructors[currentRegion].emplace_back(f_data);
			else if (f_data->type == FunctionData::OPERATOR)
				data.operators[currentRegion].emplace_back(f_data);
			else 
				data.methods[currentRegion].emplace_back(f_data);
			ctx.dispatcher->RemoveListenerDispatch(nullptr);
		} else if (typeid(ClassPolicy) == typeid(*policy)) {
			data.innerClasses[currentRegion].emplace_back(policy->Data<ClassData>());
			ctx.dispatcher->RemoveListener(nullptr);
		}
	}

protected:
	std::any DataInner() const override { return std::make_shared<ClassData>(data); }

private:
	void InitializeClassPolicyHandlers() {
		using namespace srcSAXEventDispatch;
		// start of policy
		std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> startPolicy = [this](srcSAXEventContext& ctx) {
			if (!classDepth) {
				classDepth = ctx.depth;
				data = ClassData{};
				data.lineNumber = ctx.currentLineNumber;
				std::map<std::string, std::string>::const_iterator stereotype_attr_itr = ctx.attributes.find("stereotype");
				if (stereotype_attr_itr != ctx.attributes.end()){
					std::istringstream stereostring(stereotype_attr_itr->second);
					data.stereotypes = std::set<std::string>(std::istream_iterator<std::string>(stereostring), std::istream_iterator<std::string>());
				}
				if (ctx.currentTag == "class")
					data.type = ClassData::CLASS;
				else if (ctx.currentTag == "struct")
					data.type = ClassData::STRUCT;
				data.name = nullptr;
                data.language = ctx.currentFileLanguage;
                data.filename = ctx.currentFilePath;
				CollectNameHandlers();
				CollectGenericHandlers();
				CollectSuperHanders();
				CollectBlockHanders();
			} else if ((classDepth + 3) == ctx.depth) {
				if (!classPolicy) classPolicy = new ClassPolicy{this};
				ctx.dispatcher->AddListenerDispatch(classPolicy);
			}
		};

		// end of policy
		std::function<void(srcSAXEventDispatch::srcSAXEventContext&)> endPolicy = [this](srcSAXEventContext& ctx) {
			if (classDepth && classDepth == ctx.depth) {
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
			if ((classDepth + 1) == ctx.depth) {
				if (!namePolicy) namePolicy = new NamePolicy{this};
				ctx.dispatcher->AddListenerDispatch(namePolicy);
			}
		};
		closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 1) == ctx.depth) {
				NopOpenEvents({ParserState::name});
				NopCloseEvents({ParserState::name});
			}
		};
	}

	void CollectGenericHandlers() {
		using namespace srcSAXEventDispatch;
		closeEventMap[ParserState::templates] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 1) == ctx.depth) {
				data.isGeneric = true;
			}
		};
	}

	void CollectSuperHanders() {
		using namespace srcSAXEventDispatch;
		openEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 1) == ctx.depth) {
				openEventMap[ParserState::super] = [this](srcSAXEventContext& ctx [[maybe_unused]]) {
					data.parents.emplace_back(ParentData{ "", false, ClassData::PUBLIC });
				};
				closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
					if (ctx.And({ ParserState::specifier })) {
						if (ctx.currentToken == "virtual") {
							data.parents.back().isVirtual = true;
						} else if (ctx.currentToken == "public") {
							data.parents.back().accessSpecifier = ClassData::PUBLIC;
						} else if (ctx.currentToken == "private") {
							data.parents.back().accessSpecifier = ClassData::PRIVATE;
						} else if (ctx.currentToken == "protected") {
							data.parents.back().accessSpecifier = ClassData::PROTECTED;
						}
					} else if (ctx.And({ ParserState::name })) {
						data.parents.back().name += ctx.currentToken;
					}
				};
			}
		};
		closeEventMap[ParserState::super_list] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 1) == ctx.depth) {
				NopOpenEvents({ParserState::super_list, ParserState::super});
				NopCloseEvents({ParserState::super_list, ParserState::tokenstring});
			}
		};
	}

	void CollectBlockHanders() {
        using namespace srcSAXEventDispatch;
	    openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 1) == ctx.depth) {
				NopOpenEvents({ParserState::name, ParserState::super_list, ParserState::super});
				NopCloseEvents({ParserState::name, ParserState::super_list, ParserState::tokenstring});
				// set up to listen to decl_stmt, member, and class policies
				openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
					if ((classDepth + 3) == ctx.depth) {
						if (!declPolicy) declPolicy = new DeclTypePolicy{this};
						ctx.dispatcher->AddListenerDispatch(declPolicy);
					}
				};
				std::function<void (srcSAXEventContext& ctx)> functionEvent = [this](srcSAXEventContext& ctx) {
					if ((classDepth + 3) == ctx.depth) {
						if (!functionPolicy) functionPolicy = new FunctionPolicy{this};
						ctx.dispatcher->AddListenerDispatch(functionPolicy);
					}
				};
				openEventMap[ParserState::function] = functionEvent;
				openEventMap[ParserState::functiondecl] = functionEvent;
				openEventMap[ParserState::constructor] = functionEvent;
				openEventMap[ParserState::constructordecl] = functionEvent;

				std::function<void (srcSAXEventContext& ctx)> destructorEvent = [this](srcSAXEventContext& ctx) {
					if ((classDepth + 3) == ctx.depth) {
						data.hasDestructor = true;
					}
				};
				openEventMap[ParserState::destructor] = destructorEvent;
				openEventMap[ParserState::destructordecl] = destructorEvent;
			}
		};

		// should always be in a region once block starts, so should not have to close
		openEventMap[ParserState::publicaccess] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 2) == ctx.depth) {
				currentRegion = ClassData::PUBLIC;
			}
		};

		openEventMap[ParserState::protectedaccess] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 2) == ctx.depth) {
				currentRegion = ClassData::PROTECTED;
			}
		};

		openEventMap[ParserState::privateaccess] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 2) == ctx.depth) {
				currentRegion = ClassData::PRIVATE;
			}
		};

		closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
			if ((classDepth + 1) == ctx.depth) {
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
