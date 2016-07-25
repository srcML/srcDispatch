#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>

class FunctionSignaturePolicy : public srcSAXEventDispatch::Listener{
    struct SignatureData{
        std::string nameoftype;
        std::string nameofidentifier;
        bool isConst;
        bool isReference;
        bool isPointer;
        bool isStatic;
    };
    public:
        SignatureData data;
        ~FunctionSignaturePolicy(){}
        FunctionSignaturePolicy(){InitializeEventHandlers();}
    private:
        std::string currentTypeName, currentDeclName, currentModifier, currentSpecifier;

        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;

            close_event_map[ParserState::modifier] = [this](const srcSAXEventContext& ctx) {
                    if(currentModifier == "*") {}
                    else if(currentModifier == "&") {}
                };

        }

};