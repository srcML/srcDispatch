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

class TestFunctionSignature : public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestFunctionSignature(){}
        TestFunctionSignature(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){}
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            signaturedata = *policy->Data<SignatureData>();
            datatotest.push_back(signaturedata);
        }
        void RunTest(){

            assert(datatotest.size() == 5);
            assert(datatotest[0].returnType == "void");
            assert(datatotest[0].name== "foo");
            assert(datatotest[0].returnTypeModifier == std::string());
            assert(datatotest[0].linenumber == 1);
            assert(datatotest[0].isConst == false);
            assert(datatotest[0].isMethod == false);
            assert(datatotest[0].isStatic == false);
            assert(datatotest[0].parameters.size() == 4);
            assert(datatotest[0].returnTypeNamespaces.size() == 0);
            assert(datatotest[0].functionNamespaces.size() == 0);
            assert(datatotest[0].pointerToConstReturn == false);
            assert(datatotest[0].constPointerReturn == false);
            assert(datatotest[0].hasAliasedReturn == false);

            assert(datatotest[1].returnType == "void");
            assert(datatotest[1].name== "bar");
            assert(datatotest[1].returnTypeModifier == std::string());
            assert(datatotest[1].linenumber == 2);
            assert(datatotest[1].isConst == false);
            assert(datatotest[1].isMethod == false);
            assert(datatotest[1].isStatic == true);
            assert(datatotest[1].parameters.size() == 4);
            assert(datatotest[1].returnTypeNamespaces.size() == 0);
            assert(datatotest[1].functionNamespaces.size() == 0);
            assert(datatotest[1].pointerToConstReturn == false);
            assert(datatotest[1].constPointerReturn == false);
            assert(datatotest[1].hasAliasedReturn == false);

            assert(datatotest[2].returnType == "int");
            assert(datatotest[2].name== "bloo");
            assert(datatotest[2].returnTypeModifier == "*");
            assert(datatotest[2].linenumber == 3);
            assert(datatotest[2].isConst == false);
            assert(datatotest[2].isMethod == false);
            assert(datatotest[2].isStatic == false);
            assert(datatotest[2].parameters.size() == 4);
            assert(datatotest[2].returnTypeNamespaces.size() == 0);
            assert(datatotest[2].functionNamespaces.size() == 0);
            assert(datatotest[2].pointerToConstReturn == false);
            assert(datatotest[2].constPointerReturn == false);
            assert(datatotest[2].hasAliasedReturn == true);

            assert(datatotest[3].returnType == "void");
            assert(datatotest[3].name== "bleep");
            assert(datatotest[3].returnTypeModifier == std::string());
            assert(datatotest[3].linenumber == 4);
            assert(datatotest[3].isConst == true);
            assert(datatotest[3].isMethod == true);
            assert(datatotest[3].isStatic == false);
            assert(datatotest[3].parameters.size() == 4);
            assert(datatotest[3].returnTypeNamespaces.size() == 0);
            assert(datatotest[3].functionNamespaces.size() == 0);
            assert(datatotest[3].pointerToConstReturn == false);
            assert(datatotest[3].constPointerReturn == false);
            assert(datatotest[3].hasAliasedReturn == false);
            assert(datatotest[3].nameOfContainingClass == "testclass");

            assert(datatotest[4].returnType == "object");
            assert(datatotest[4].name== "bloo");
            assert(datatotest[4].returnTypeModifier == "*");
            assert(datatotest[4].linenumber == 5);
            assert(datatotest[4].isConst == false);
            assert(datatotest[4].isMethod == false);
            assert(datatotest[4].isStatic == true);
            assert(datatotest[4].parameters.size() == 3);
            assert(datatotest[4].returnTypeNamespaces.size() == 2);
            assert(datatotest[4].functionNamespaces.size() == 1);
            assert(datatotest[4].pointerToConstReturn == true);
            assert(datatotest[4].constPointerReturn == true);
            assert(datatotest[4].hasAliasedReturn == true);

        }
    protected:
        void * DataInner() const override {
            return (void*)0; //To silence the warning
        }
    private:
        SignatureData signaturedata;
        std::vector<SignatureData> datatotest;
};

int main(int argc, char** filename){
    std::string codestr = "void foo(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}\n"
                          "static void bar(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}\n"
                          "int* bloo(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}\n"
                          "class testclass{void bleep(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee)const{}};\n"
                          "static const GameDes::std::object* const std::bloo(Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}";
    std::string srcmlstr = StringToSrcML(codestr);

    TestFunctionSignature sigData;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<FunctionSignaturePolicy> handler {&sigData};
    control.parse(&handler); //Start parsing
    sigData.RunTest();
}