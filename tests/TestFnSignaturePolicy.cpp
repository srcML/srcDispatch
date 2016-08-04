#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <FunctionSignaturePolicy.hpp>
#include <cassert>
#include <srcml.h>
std::string StringToSrcML(std::string str){
    struct srcml_archive* archive;
    struct srcml_unit* unit;
    size_t size = 0;

    char *ch = new char[str.size()];

    archive = srcml_archive_create();
    srcml_archive_enable_option(archive, SRCML_OPTION_POSITION);
    srcml_archive_write_open_memory(archive, &ch, &size);

    unit = srcml_unit_create(archive);
    srcml_unit_set_language(unit, SRCML_LANGUAGE_CXX);
    srcml_unit_set_filename(unit, "testsrcType.cpp");

    srcml_unit_parse_memory(unit, str.c_str(), str.size());
    srcml_archive_write_unit(archive, unit);
    
    srcml_unit_free(unit);
    srcml_archive_close(archive);
    srcml_archive_free(archive);
    //TrimFromEnd(ch, size);
    return std::string(ch);
}

class TestFunctionSignature : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestFunctionSignature(){}
        TestFunctionSignature(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
            parampolicy.AddListener(this);
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            signaturedata = *policy->Data<FunctionSignaturePolicy::SignatureData>();
            datatotest.push_back(signaturedata);
        }
        void RunTest(){
            assert(datatotest.size() == 5);
            assert(datatotest[0].returnType == "void");
            assert(datatotest[0].functionName == "foo");
            assert(datatotest[0].returnTypeModifier == std::string());
            assert(datatotest[0].linenumber == 1);
            assert(datatotest[0].isConst == false);
            assert(datatotest[0].isMethod == false);
            assert(datatotest[0].isStatic == false);
            assert(datatotest[0].parameters.size() == 4);
            assert(datatotest[0].returnTypeNamespaces.size() == 0);
            assert(datatotest[0].functionNamespaces.size() == 0);

            assert(datatotest[1].returnType == "void");
            assert(datatotest[1].functionName == "bar");
            assert(datatotest[1].returnTypeModifier == std::string());
            assert(datatotest[1].linenumber == 2);
            assert(datatotest[1].isConst == false);
            assert(datatotest[1].isMethod == false);
            assert(datatotest[1].isStatic == true);
            assert(datatotest[1].parameters.size() == 4);
            assert(datatotest[1].returnTypeNamespaces.size() == 0);
            assert(datatotest[1].functionNamespaces.size() == 0);

            assert(datatotest[2].returnType == "int");
            assert(datatotest[2].functionName == "bloo");
            assert(datatotest[2].returnTypeModifier == "*");
            assert(datatotest[2].linenumber == 3);
            assert(datatotest[2].isConst == false);
            assert(datatotest[2].isMethod == false);
            assert(datatotest[2].isStatic == false);
            assert(datatotest[2].parameters.size() == 4);
            assert(datatotest[2].returnTypeNamespaces.size() == 0);
            assert(datatotest[2].functionNamespaces.size() == 0);

            assert(datatotest[3].returnType == "void");
            assert(datatotest[3].functionName == "bleep");
            assert(datatotest[3].returnTypeModifier == std::string());
            assert(datatotest[3].linenumber == 4);
            assert(datatotest[3].isConst == true);
            assert(datatotest[3].isMethod == false);
            assert(datatotest[3].isStatic == false);
            assert(datatotest[3].parameters.size() == 4);
            assert(datatotest[3].returnTypeNamespaces.size() == 0);
            assert(datatotest[3].functionNamespaces.size() == 0);

            assert(datatotest[4].returnType == "object");
            assert(datatotest[4].functionName == "bloo");
            assert(datatotest[4].returnTypeModifier == "*");
            assert(datatotest[4].linenumber == 5);
            assert(datatotest[4].isConst == false);
            assert(datatotest[4].isMethod == false);
            assert(datatotest[4].isStatic == false);
            assert(datatotest[4].parameters.size() == 4);
            assert(datatotest[4].returnTypeNamespaces.size() == 2);
            assert(datatotest[4].functionNamespaces.size() == 1);

        }
    protected:
        void * DataInner() const {
            return (void*)0; //To silence the warning
        }
    private:
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::function] = [this](srcSAXEventContext& ctx) {
                ctx.dispatcher->AddListener(&parampolicy);
            };
        }
        FunctionSignaturePolicy parampolicy;
        FunctionSignaturePolicy::SignatureData signaturedata;
        std::vector<FunctionSignaturePolicy::SignatureData> datatotest;
};

int main(int argc, char** filename){
    std::string codestr = "void foo(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}\n"
                          "static void bar(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}\n"
                          "int* bloo(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}\n"
                          "class{void bleep(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee)const{}};\n"
                          "GameDes::std::object* std::bloo(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}";
    std::string srcmlstr = StringToSrcML(codestr);

    TestFunctionSignature sigData;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<TestFunctionSignature> handler {&sigData};
    control.parse(&handler); //Start parsing
    sigData.RunTest();
}