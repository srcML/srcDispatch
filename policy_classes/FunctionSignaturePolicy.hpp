#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <exception>

class FunctionSignaturePolicy : public srcSAXEventDispatch::EventListener{
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

            closeEventMap[ParserState::modifier] = [this](srcSAXEventContext& ctx) {
                    if(currentModifier == "*") {}
                    else if(currentModifier == "&") {}
                };

        }

};