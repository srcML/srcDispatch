#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>

#include <string>
#include <vector>

class NamePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct NameData {

        std::string name;
        std::vector<NameData *> names;
        std::vector<NameData *> templateArguments;
        std::vector<std::string> arrayIndices;

        friend std::ostream & operator<<(std::ostream & out, const NameData & nameData) {

            return out << nameData.name;

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
    virtual void Notify(const PolicyDispatcher * policy) override {

        data.names.push_back(policy->Data<NameData>());

    }

private:

    void InitializeNamePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(!nameDepth) {

                nameDepth = ctx.depth;
                data = NameData{};

            } else if((nameDepth + 1) == ctx.depth) {

                // namePolicy = new NamePolicy{this};
                // ctx.AddListener(namePolicy);

            }

        };

        // end of policy
        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                nameDepth = 0;
                NotifyAll();
                InitializeNamePolicyHandlers();

            } else if(nameDepth && (nameDepth + 1) == ctx.depth) {

                // ctx.RemoveListener(namePolicy);
                // delete namePolicy;
                // namePolicy = nullptr;

            }
           
        };

        closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                data.name += ctx.currentToken;

            }

        };

    }

    void TemplateHandler() {}

    void ArrayIndicesHandler() {}

};
