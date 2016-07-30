#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <srcSAXEventDispatch.hpp>
#include <FunctionSignaturePolicy.hpp>
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
    srcml_write_unit(archive, unit);
    
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
            signaturedata = policy->Data<FunctionSignaturePolicy::SignatureData>();
            datatotest.push_back(signaturedata);
        }
    protected:
        void * DataInner() const {
            return (void*)0;
        }
    private:
        void InitializeEventHandlers(){
            using namespace srcSAXEventDispatch;
            openEventMap[ParserState::function] = [this](srcSAXEventContext& ctx) {
                ctx.AddListener(&parampolicy);
            };
            closeEventMap[ParserState::functionblock] = [this](srcSAXEventContext& ctx) {
                ctx.RemoveListener(&parampolicy);
                RunTest();
            };
        }
        void RunTest(){
            for(FunctionSignaturePolicy::SignatureData* testdata : datatotest){
                //do the thing
            }
        }    
        FunctionSignaturePolicy parampolicy;
        FunctionSignaturePolicy::SignatureData* signaturedata;
        std::vector<FunctionSignaturePolicy::SignatureData*> datatotest;
};

int main(int argc, char** filename){
    std::string codestr = "void foo(int abc, Object<int> onetwothree, Object* DoReiMe, const Object* aybeecee){}";
    std::string srcmlstr = StringToSrcML(codestr);

    TestFunctionSignature paramData;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<TestFunctionSignature> handler {&paramData};
    control.parse(&handler); //Start parsing
}