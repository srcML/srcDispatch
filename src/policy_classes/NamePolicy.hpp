#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicy.hpp>
#include <TemplateArgumentPolicy.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_NAME_POLICY_HPP
#define INCLUDED_NAME_POLICY_HPP

class NamePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct NameData {

        std::string name;
        std::vector<NameData *> names;
        // std::vector<TemplateArgumentPolicy::TemplateArgumentData *> templateArguments;
        std::vector<std::string> arrayIndices;

        friend std::ostream & operator<<(std::ostream & out, const NameData & nameData) {

            if(!nameData.name.empty()) {
                out << nameData.name;
            }

            for(size_t pos = 0; pos < nameData.names.size(); ++pos) {

                if(pos != 0) out << "::";
                out << (*nameData.names[pos]);

            }

            // if(!nameData.templateArguments.empty()) {
            //     out << '<';
            //     for(const std::string & arg : nameData.templateArguments) {
            //         out << arg;
            //     }
            //     out << '>';
            // }

            for(const std::string & index : nameData.arrayIndices) {
                out << '[' << index << ']';
            }

            return out;


        }

    };

private:

    NameData data;
    std::size_t nameDepth;

    NamePolicy * namePolicy;

public:


    NamePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          nameDepth(0),
          namePolicy(nullptr) { 
    
        InitializeNamePolicyHandlers();

    }

protected:
    void * DataInner() const override {

        return new NameData(data);

    }
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if((nameDepth + 1) == ctx.depth)
            data.names.push_back(policy->Data<NameData>());

    }

private:

    void InitializeNamePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(!nameDepth) {

                NopCloseEvents({ParserState::tokenstring});

                nameDepth = ctx.depth;
                data = NameData{};
                namePolicy = new NamePolicy{this};
                ctx.AddListenerNoDispatch(namePolicy);

                CollectTemplateArgumentsHandlers();
                CollectArrayIndicesHandlers();

            }

        };

        // end of policy
        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                nameDepth = 0;
                if(namePolicy) {
                    ctx.RemoveListenerNoDispatch(namePolicy);
                    delete namePolicy;
                    namePolicy = nullptr;
                }

                NotifyAll(ctx);
                InitializeNamePolicyHandlers();

            }
           
        };

        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                data.name += ctx.currentToken;

            }

        };

    }

    void CollectTemplateArgumentsHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::argumentlist] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                // data.templateArguments.push_back(nullptr);
                openEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {

                    size_t num_elements = ctx.elementStack.size();
                    if(nameDepth && (nameDepth + 2) == ctx.depth && num_elements > 1 && ctx.elementStack[num_elements - 2] == "argument_list") {}

                };

                closeEventMap[ParserState::argument] = [this](srcSAXEventContext& ctx) {

                    size_t num_elements = ctx.elementStack.size();
                    if(nameDepth && (nameDepth + 2) == ctx.depth && num_elements > 0 && ctx.elementStack.back() == "argument_list") {}

                };

            }

        };

        closeEventMap[ParserState::argumentlist] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::argumentlist, ParserState::argument});
                NopCloseEvents({ParserState::argumentlist, ParserState::argument});

            }

        };

    }

    void CollectArrayIndicesHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                data.arrayIndices.push_back(std::string());

            }

        };

        closeEventMap[ParserState::index] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && (nameDepth + 1) == ctx.depth) {

                NopOpenEvents({ParserState::expr});
                NopCloseEvents({ParserState::expr});

            }

        };

        openEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {

            size_t num_elements = ctx.elementStack.size();
            if(nameDepth && (nameDepth + 2) == ctx.depth && num_elements > 1 && ctx.elementStack[num_elements - 2] == "index") {

                closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) { data.arrayIndices.back() += ctx.currentToken; };

            }

        };

        closeEventMap[ParserState::expr] = [this](srcSAXEventContext& ctx) {

            size_t num_elements = ctx.elementStack.size();
            if(nameDepth && (nameDepth + 2) == ctx.depth && num_elements > 0 && ctx.elementStack.back() == "index") {

                NopCloseEvents({ParserState::tokenstring});

            }

        };

    }

};

#endif
