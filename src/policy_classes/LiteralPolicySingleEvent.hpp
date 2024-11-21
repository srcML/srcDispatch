/**
 * @file LiteralPolicySingleEvent.hpp
 *
 *
 */
#ifndef INCLUDED_LITERAL_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_LITERAL_POLICY_SINGLE_EVENT_HPP

#include <srcSAXController.hpp>
#include <srcDispatcherSingleEvent.hpp>
#include <srcDispatchUtilities.hpp>

#include <string>
#include <iostream>

struct LiteralData {

    unsigned int startLineNumber;
    std::string literal;

    friend std::ostream& operator<<(std::ostream& out, const LiteralData& literalData) {
        return out << literalData.literal;
    }
};

class LiteralPolicy :
public srcDispatch::EventListener,
public srcDispatch::PolicyDispatcher,
public srcDispatch::PolicyListener {

private:
    LiteralData data;
    std::size_t  literalDepth;

public:
    LiteralPolicy(std::initializer_list<srcDispatch::PolicyListener*> listeners)
        : srcDispatch::PolicyDispatcher(listeners),
          data{},
          literalDepth(0) {
        InitializeLiteralPolicyHandlers();
    }

    ~LiteralPolicy() {
    }

protected:
    std::any DataInner() const { return std::make_shared<LiteralData>(data); }
    void Notify(const PolicyDispatcher* policy, const srcDispatch::srcSAXEventContext& ctx) {}
    void NotifyWrite(const PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

private:
    void InitializeLiteralPolicyHandlers() {
        using namespace srcDispatch;

        openEventMap[ParserState::literal] = [this](srcSAXEventContext& ctx) {                     
            if (!literalDepth) {                          
                literalDepth = ctx.depth;                 
                data = LiteralData{};                     
                data.startLineNumber = ctx.currentLineNumber; 
                CollectTokenHandlers();                       
            }                                                 
        }; 

        // end of policy
        closeEventMap[ParserState::literal] =[this](srcSAXEventContext& ctx) {
            if (literalDepth && literalDepth == ctx.depth) {
                literalDepth = 0;
                NotifyAll(ctx);
                InitializeLiteralPolicyHandlers();
            }
        };
    }

    void CollectTokenHandlers() {
        using namespace srcDispatch;
        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {
            data.literal += ctx.currentToken;
        };
    }
};

#endif
