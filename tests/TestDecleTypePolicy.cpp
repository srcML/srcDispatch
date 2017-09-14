#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
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
	srcml_archive_write_unit(archive, unit);
	
	srcml_unit_free(unit);
	srcml_archive_close(archive);
	srcml_archive_free(archive);
	//TrimFromEnd(ch, size);
	return std::string(ch);
}

class TestDeclType : public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestDeclType(){}
        TestDeclType(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){}
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            decltypedata = *policy->Data<DeclData>();
            datatotest.push_back(decltypedata);
        }
		void RunTest(){
			assert(datatotest.size() == 6);
			assert(datatotest[0].nameOfType == "int");
			assert(datatotest[0].nameOfIdentifier == "abc");
			assert(datatotest[0].linenumber == 1);
			assert(datatotest[0].isConstValue == false);
			assert(datatotest[0].isConstAlias == false);
			assert(datatotest[0].isReference == true);
			assert(datatotest[0].isPointer == false);
			assert(datatotest[0].isStatic == false);
			assert(datatotest[0].namespaces.empty());
			assert(datatotest[0].isClassMember == true);
			assert(datatotest[0].nameOfContainingClass == "testclass");
			assert(datatotest[0].nameOfContainingFunction == "");
			assert(datatotest[0].nameOfContainingFile == "testsrcType.cpp");

			assert(datatotest[1].nameOfType == "Object");
			assert(datatotest[1].nameOfIdentifier == "onetwothree");
			assert(datatotest[1].linenumber == 1);
			assert(datatotest[1].isConstValue == false);
			assert(datatotest[1].isConstAlias == false);
			assert(datatotest[1].isReference == false);
			assert(datatotest[1].isPointer == false);
			assert(datatotest[1].isStatic == false);
			assert(datatotest[1].namespaces.empty());
			assert(datatotest[1].nameOfContainingClass == "testclass");
			assert(datatotest[1].nameOfContainingFunction == "");
			assert(datatotest[1].nameOfContainingFile == "testsrcType.cpp");

			assert(datatotest[2].nameOfType == "Object");
			assert(datatotest[2].nameOfIdentifier == "DoReiMe");
			assert(datatotest[2].linenumber == 1);
			assert(datatotest[2].isConstValue == false);
			assert(datatotest[2].isConstAlias == false);
			assert(datatotest[2].isReference == false);
			assert(datatotest[2].isPointer == true);
			assert(datatotest[2].isStatic == true);
			assert(datatotest[2].namespaces.empty());
			assert(datatotest[2].nameOfContainingClass == "");
			assert(datatotest[2].nameOfContainingFunction == "foo");
			assert(datatotest[2].nameOfContainingFile == "testsrcType.cpp");

			assert(datatotest[3].nameOfType == "Object");
			assert(datatotest[3].nameOfIdentifier == "aybeecee");
			assert(datatotest[3].linenumber == 1);
			assert(datatotest[3].isConstAlias == true);
			assert(datatotest[3].isConstValue == true);
			assert(datatotest[3].isReference == false);
			assert(datatotest[3].isPointer == true);
			assert(datatotest[3].isStatic == false);
			assert(datatotest[3].namespaces.empty());
			assert(datatotest[3].nameOfContainingClass == "");
			assert(datatotest[3].nameOfContainingFunction == "foo");
			assert(datatotest[3].nameOfContainingFile == "testsrcType.cpp");

			assert(datatotest[4].nameOfType == "vector");
			assert(datatotest[4].nameOfIdentifier == "spaces");
			assert(datatotest[4].linenumber == 2);
			assert(datatotest[4].isConstValue == false);
			assert(datatotest[4].isConstAlias == false);
			assert(datatotest[4].isReference == false);
			assert(datatotest[4].isPointer == false);
			assert(datatotest[4].isStatic == false);
			assert(datatotest[4].namespaces.size() == 2);
			assert(datatotest[4].nameOfContainingClass == "");
			assert(datatotest[4].nameOfContainingFunction == "foo");
			assert(datatotest[4].nameOfContainingFile == "testsrcType.cpp");

			assert(datatotest[5].nameOfType == "int");
			assert(datatotest[5].nameOfIdentifier == "ab");
			assert(datatotest[5].linenumber == 2);
			assert(datatotest[5].isConstValue == false);
			assert(datatotest[5].isConstAlias == true);
			assert(datatotest[5].isReference == false);
			assert(datatotest[5].isPointer == true);
			assert(datatotest[5].isStatic == false);
			assert(datatotest[5].namespaces.empty());
			assert(datatotest[5].isClassMember == false);
			assert(datatotest[5].usesSubscript == true);
			assert(datatotest[5].nameOfContainingClass == "");
			assert(datatotest[5].nameOfContainingFunction == "");
			assert(datatotest[5].nameOfContainingFile == "testsrcType.cpp");
		}
    protected:
        void * DataInner() const override {
            return (void*)0; //To silence the warning
        }
    private:

        DeclTypePolicy declpolicy;
        DeclData decltypedata;
        std::vector<DeclData> datatotest;
};

int main(int argc, char** filename){
	std::string codestr = "class testclass {int& abc; Object<int> onetwothree; void foo(){static Object* DoReiMe; const Object* const aybeecee;\n nlp::std::vector<std::string> spaces;}}; int* const ab[5];";
	std::string srcmlstr = StringToSrcML(codestr);

    TestDeclType decltypedata;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<DeclTypePolicy> handler {&decltypedata};
    control.parse(&handler); //Start parsing
    decltypedata.RunTest();
}