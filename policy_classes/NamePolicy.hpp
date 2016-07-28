#include <srcSAXEventDispatch.hpp>
#include <srcSAXEventDispatchUtilities.hpp>

#include <string>
#include <vector>

class NamePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct NameData {

        std::string name;
        bool isCompound;
        std::vector<NameData> templateArguments;
        std::vector<std::string> arrayIndices;

    };

private:

    NameData data;
    std::size_t nameDepth;
    NamePolicy * policy;

public:


    NamePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          nameDepth(0) { 
    
        InitializeNamePolicyHandlers();

    }

protected:
    void * DataInner() const override {

        return new NameData(data);

    }
    virtual void Notify(const PolicyDispatcher * policy) override {


    }

private:

    void InitializeNamePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(!nameDepth) {

                nameDepth = ctx.depth;
                data = NameData{};
                data.isCompound = false;

            }

        };

        // end of policy
        closeEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(nameDepth && nameDepth == ctx.depth) {

                nameDepth = 0;
                NotifyAll();
                InitializeNamePolicyHandlers();

            }
           
        };


    }

    void InnerNameHanders() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(ctx.depth == (nameDepth + 1)) {

                policy = new NamePolicy{this};


            }

        };

    }

    void TemplateHandler() {}

    void ArrayIndicesHandler() {}

};
