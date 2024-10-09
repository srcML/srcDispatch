/**
 * @file FunctionPolicySingleEvent.hpp
 *
 * MODIFIED FOR STEREOCODE
 *
 */
#ifndef INCLUDED_FUNCTION_POLICY_SINGE_EVENT_HPP
#define INCLUDED_FUNCTION_POLICY_SINGE_EVENT_HPP

#include <srcDispatchUtilities.hpp>

#include <NamePolicySingleEvent.hpp>
#include <DeclTypePolicySingleEvent.hpp>
#include <TypePolicySingleEvent.hpp>
#include <ParamTypePolicySingleEvent.hpp>
#include <ExpressionPolicySingleEvent.hpp>
#include <ReturnPolicySingleEvent.hpp>

#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <iterator>

struct FunctionData {
    enum FunctionType { CONSTRUCTOR, DESTRUCTOR, OPERATOR, FUNCTION };

    unsigned int lineNumber;
    std::string                   language;
    std::string                   filename;
    FunctionType                  type;
    bool                          isDecl;
    std::shared_ptr<TypeData>                    returnType;
    std::shared_ptr<NameData>                    name;
    std::vector<std::shared_ptr<ParamTypeData>>  parameters;
    std::vector<std::shared_ptr<DeclTypeData>>   locals;            //Local variables
    std::vector<std::shared_ptr<ExpressionData>> returnExpressions; //Expressions returned
    std::vector<std::shared_ptr<ExpressionData>> expressions;       //All other exprs
    std::set<std::string>         stereotypes;

    bool isVirtual;
    bool isPureVirtual;
    bool isConst;
    bool isStatic;
    bool isInline;
    bool isFinal;
    bool isOverride;
    bool isConstExpr;
    bool isDelete;

    std::string ToString() const {
        std::string signature = name->ToString();
        signature += '(';
        for(std::size_t pos = 0; pos < parameters.size(); ++pos) {
            if (pos > 0) {
                signature += ", ";
            }
            signature += parameters[pos]->type->ToString();
        }
        signature += ')';
        if (isConst) {
            signature += " const";
        }
        return signature;
    }

    friend std::ostream & operator<<(std::ostream & out, const FunctionData & functionData) {
        if (functionData.returnType){
            out << *functionData.returnType << ' ' << *functionData.name;
        }
        out << '(';
        for(std::size_t pos = 0; pos < functionData.parameters.size(); ++pos) {
            if (pos != 0)
                out << ", ";
            out << *functionData.parameters[pos];
        }
        out << ')';
        return out;
    }
};


class FunctionPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    FunctionData     data;
    std::size_t      functionDepth;
    
    TypePolicy       *typePolicy;
    NamePolicy       *namePolicy;
    ParamTypePolicy  *paramPolicy;
    DeclTypePolicy   *declstmtPolicy;
    ReturnPolicy     *returnPolicy;
    ExpressionPolicy *expressionPolicy;

public:
    FunctionPolicy(std::initializer_list<srcDispatch::PolicyListener *> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          functionDepth(0),
          typePolicy(nullptr),
          namePolicy(nullptr),
          paramPolicy(nullptr),
          declstmtPolicy(nullptr),
          expressionPolicy(nullptr),
          returnPolicy(nullptr) {
        InitializeFunctionPolicyHandlers();
    }

    ~FunctionPolicy() {
        if (typePolicy)       delete typePolicy;
        if (namePolicy)       delete namePolicy;
        if (paramPolicy)      delete paramPolicy;
        if (declstmtPolicy)   delete declstmtPolicy;
        if (returnPolicy)     delete returnPolicy;
        if (expressionPolicy) delete expressionPolicy;
    }

protected:
    std::any DataInner() const override { return std::make_shared<FunctionData>(data); }

    void NotifyWrite(const PolicyDispatcher * policy [[maybe_unused]], srcDispatch::srcSAXEventContext & ctx [[maybe_unused]]) override {} //doesn't use other parsers

    virtual void Notify(const PolicyDispatcher * policy, const srcDispatch::srcSAXEventContext & ctx) override {
        if (typeid(TypePolicy) == typeid(*policy)) {
            data.returnType = policy->Data<TypeData>();
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(NamePolicy) == typeid(*policy)) {
            data.name = policy->Data<NameData>(); 
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ParamTypePolicy) == typeid(*policy)) {
            data.parameters.push_back(policy->Data<ParamTypeData>()); 
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(DeclTypePolicy) == typeid(*policy)) {
            std::shared_ptr<std::vector<std::shared_ptr<DeclTypeData>>> decl_data = policy->Data<std::vector<std::shared_ptr<DeclTypeData>>>();
            for(std::shared_ptr<DeclTypeData> decl : *decl_data) {
                data.locals.push_back(decl);
            }
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ReturnPolicy) == typeid(*policy)) {
            data.returnExpressions.push_back(policy->Data<ExpressionData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        } else if (typeid(ExpressionPolicy) == typeid(*policy)) {
            data.expressions.push_back(policy->Data<ExpressionData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);
        }
    }

private:
    void InitializeFunctionPolicyHandlers() {
        using namespace srcDispatch;
        // start of policy
        std::function<void (srcSAXEventContext& ctx)> startFunction = [this](srcSAXEventContext& ctx) {
            if (!functionDepth) {
                functionDepth = ctx.depth;
                data = FunctionData{};
                data.lineNumber = ctx.currentLineNumber;
                data.language = ctx.currentFileLanguage;
                data.filename = ctx.currentFilePath;
                std::map<std::string, std::string>::const_iterator stereotype_attr_itr = ctx.attributes.find("stereotype");
                if (stereotype_attr_itr != ctx.attributes.end()){
                    std::istringstream stereostring(stereotype_attr_itr->second);
                    data.stereotypes = std::set<std::string>(std::istream_iterator<std::string>(stereostring), std::istream_iterator<std::string>());
                }
                if (ctx.currentTag == "function" || ctx.currentTag == "function_decl") {
                    if (ctx.isOperator)
                        data.type = FunctionData::OPERATOR;
                    else
                        data.type = FunctionData::FUNCTION;
                } else if (ctx.currentTag == "constructor" || ctx.currentTag == "constructor_decl") {
                    data.type = FunctionData::CONSTRUCTOR;
                } else if (ctx.currentTag == "destructor" || ctx.currentTag == "destructor_decl") {
                    data.type = FunctionData::DESTRUCTOR;
                }

                data.isDecl = ctx.currentTag == "function_decl" 
                           || ctx.currentTag == "constructor_decl"
                           || ctx.currentTag == "destructor_decl";

                CollectXMLAttributeHandlers();
                CollectTypeHandlers();
                CollectNameHandlers();
                CollectParameterHandlers();
                CollectOtherHandlers();
                CollectDeclstmtHandlers();
                CollectReturnHandlers();
                CollectExpressionHandlers();
            }
        };

        // end of policy
        std::function<void (srcSAXEventContext& ctx)> endFunction = [this](srcSAXEventContext& ctx) {
            if (functionDepth && functionDepth == ctx.depth) {
                functionDepth = 0;
                NotifyAll(ctx);
                InitializeFunctionPolicyHandlers();
            }
        };

        openEventMap[ParserState::function]        = startFunction;
        openEventMap[ParserState::functiondecl]    = startFunction;
        openEventMap[ParserState::constructor]     = startFunction;
        openEventMap[ParserState::constructordecl] = startFunction;
        openEventMap[ParserState::destructor]      = startFunction;
        openEventMap[ParserState::destructordecl]  = startFunction;

        closeEventMap[ParserState::function]        = endFunction;
        closeEventMap[ParserState::functiondecl]    = endFunction;
        closeEventMap[ParserState::constructor]     = endFunction;
        closeEventMap[ParserState::constructordecl] = endFunction;
        closeEventMap[ParserState::destructor]      = endFunction;
        closeEventMap[ParserState::destructordecl]  = endFunction;
    }

    void CollectXMLAttributeHandlers() {}

    void CollectTypeHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {
            if (functionDepth && (functionDepth + 1) == ctx.depth) {
                if (!typePolicy) typePolicy = new TypePolicy{this};
                ctx.dispatcher->AddListenerDispatch(typePolicy);
            }
        };
    }

    void CollectNameHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {
            if (functionDepth && (functionDepth + 1) == ctx.depth) {
                if (!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy);
            }
        };
    }

    void CollectParameterHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
            if (functionDepth && (functionDepth + 1) == ctx.depth) {
                openEventMap[ParserState::parameter] = [this](srcSAXEventContext& ctx) {
                    if (functionDepth && (functionDepth + 2) == ctx.depth) {
                        if (!paramPolicy) paramPolicy = new ParamTypePolicy{this};
                        ctx.dispatcher->AddListenerDispatch(paramPolicy);
                    }
                };
            }
        };

        closeEventMap[ParserState::parameterlist] = [this](srcSAXEventContext& ctx) {
            if (functionDepth && (functionDepth + 1) == ctx.depth) {
                NopOpenEvents({ParserState::parameter});
            }
        };
    }

    void CollectReturnHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::returnstmt] = [this](srcSAXEventContext& ctx) {
            if (!returnPolicy) returnPolicy = new ReturnPolicy{this};
            ctx.dispatcher->AddListenerDispatch(returnPolicy);
        };
    }

    void CollectExpressionHandlers() {
        using namespace srcDispatch;
        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {
            if (!expressionPolicy) expressionPolicy = new ExpressionPolicy{this};
            ctx.dispatcher->AddListenerDispatch(expressionPolicy);
        };
    }

    void CollectOtherHandlers() {
        using namespace srcDispatch;
        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
             if (functionDepth && (functionDepth + 1) == ctx.depth) {
                if (ctx.And({ParserState::specifier})) {
                    if (ctx.currentToken == "virtual")
                        data.isVirtual = true;
                    else if (ctx.currentToken == "static")
                        data.isStatic = true;
                    else if (ctx.currentToken == "const")
                        data.isConst = true;
                    else if (ctx.currentToken == "final")
                        data.isFinal = true;
                    else if (ctx.currentToken == "override")
                        data.isOverride = true;
                    else if (ctx.currentToken == "delete")
                        data.isDelete = true;
                    else if (ctx.currentToken == "inline")
                        data.isInline = true;
                    else if (ctx.currentToken == "constexpr")
                        data.isConstExpr = true;
                } else if (ctx.And({ParserState::literal})) {
                    data.isPureVirtual = true;
                }
             }
        };
    }

    /** @todo Will not work with local classes. */
    /** @todo May need to add optimization that ignores declaration statement initialization. */
    void CollectDeclstmtHandlers(){
        using namespace srcDispatch;
        openEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if (functionDepth && (functionDepth + 1) == ctx.depth) {
                openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
                    if (!declstmtPolicy) declstmtPolicy = new DeclTypePolicy{this};
                    ctx.dispatcher->AddListenerDispatch(declstmtPolicy);
                };
            }
        };
        
        closeEventMap[ParserState::block] = [this](srcSAXEventContext& ctx) {
            if (functionDepth && (functionDepth + 1) == ctx.depth) {
                NopOpenEvents({ParserState::declstmt});
            }
        };
    }

};

#endif
