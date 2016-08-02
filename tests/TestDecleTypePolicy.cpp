#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <srcSAXEventDispatch.hpp>
#include <DeclTypePolicy.hpp>
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
	srcml_write_unit(archive, unit);
	
	srcml_unit_free(unit);
	srcml_archive_close(archive);
	srcml_archive_free(archive);
	//TrimFromEnd(ch, size);
	return std::string(ch);
}

class TestDeclType : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestDeclType(){}
        TestDeclType(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){
            InitializeEventHandlers();
            declpolicy.AddListener(this);
        }
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            decltypedata = *policy->Data<DeclTypePolicy::DeclTypeData>();
            datatotest.push_back(decltypedata);
        }
		void RunTest(){
			assert(datatotest.size() == 4);
			assert(datatotest[0].nameoftype == "int");
			assert(datatotest[0].nameofidentifier == "abc");
			assert(datatotest[0].linenumber == 1);
			assert(datatotest[0].isConst == false);
			assert(datatotest[0].isReference == true);
			assert(datatotest[0].isPointer == false);
			assert(datatotest[0].isStatic == false);

			assert(datatotest[1].nameoftype == "Object");
			assert(datatotest[1].nameofidentifier == "onetwothree");
			assert(datatotest[1].linenumber == 1);
			assert(datatotest[1].isConst == false);
			assert(datatotest[1].isReference == false);
			assert(datatotest[1].isPointer == false);
			assert(datatotest[1].isStatic == false);

			assert(datatotest[2].nameoftype == "Object");
			assert(datatotest[2].nameofidentifier == "DoReiMe");
			assert(datatotest[2].linenumber == 1);
			assert(datatotest[2].isConst == false);
			assert(datatotest[2].isReference == false);
			assert(datatotest[2].isPointer == true);
			assert(datatotest[2].isStatic == true);

			assert(datatotest[3].nameoftype == "Object");
			assert(datatotest[3].nameofidentifier == "aybeecee");
			assert(datatotest[3].linenumber == 1);
			assert(datatotest[3].isConst == true);
			assert(datatotest[3].isReference == false);
			assert(datatotest[3].isPointer == true);
			assert(datatotest[3].isStatic == false);
		}
    protected:
        void * DataInner() const {
            return (void*)0; //To silence the warning
        }
    private:
		void InitializeEventHandlers(){
    		using namespace srcSAXEventDispatch;
        	openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {
            	ctx.AddListener(&declpolicy);
        	};
		}
        DeclTypePolicy declpolicy;
        DeclTypePolicy::DeclTypeData decltypedata;
        std::vector<DeclTypePolicy::DeclTypeData> datatotest;
};

int main(int argc, char** filename){
	std::string codestr = "void foo(){int& abc; Object<int> onetwothree; static Object* DoReiMe; const Object* aybeecee;}";
	std::string srcmlstr = StringToSrcML(codestr);

    TestDeclType decltypedata;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<TestDeclType> handler {&decltypedata};
    control.parse(&handler); //Start parsing
    decltypedata.RunTest();
}