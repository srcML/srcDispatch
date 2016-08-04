#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <srcSAXEventDispatch.hpp>
#include <FunctionCallPolicy.hpp>
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

class TestCalls : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestCalls(){}
        TestCalls(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
            callpolicy.AddListener(this);
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            calldata = *policy->Data<CallPolicy::CallData>();
            datatotest.push_back(calldata);
        }
		void RunTest(){
			assert(datatotest[0].fnName == "bin"); //TODO: Fix, figure out way to test.
		}
    protected:
        void * DataInner() const {
            return (void*)0; //To silence the warning
        }
    private:
		void InitializeEventHandlers(){
    		using namespace srcSAXEventDispatch;
        	openEventMap[ParserState::call] = [this](srcSAXEventContext& ctx) {
            	ctx.dispatcher->AddListener(&callpolicy);
        	};
		}
        CallPolicy callpolicy;
        CallPolicy::CallData calldata;
        std::vector<CallPolicy::CallData> datatotest;

};

int main(int argc, char** filename){
	std::string codestr = "void foo(){foo(bar, baz, bin(), beep);}";
	std::string srcmlstr = StringToSrcML(codestr);
	
    TestCalls calldata;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<TestCalls> handler {&calldata};
    control.parse(&handler); //Start parsing
    calldata.RunTest();
}