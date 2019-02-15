#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <ExprPolicy.hpp>
#include <cassert>
#include <srcml.h>
std::string StringToSrcML(std::string str){
	struct srcml_archive* archive;
	struct srcml_unit* unit;
	size_t size = 0;

	char *ch = 0;

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
	ch[size-1] = 0;
	return std::string(ch);
}

class TestExpr : public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestExpr(){}
        TestExpr(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){}
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            exprdata = *policy->Data<ExprPolicy::ExprDataSet>();
            datatotest.push_back(exprdata);
        }
        void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {
        }
		void RunTest(){
			
		}
    protected:
        void * DataInner() const override {
            return (void*)0; //To silence the warning
        }
    private:
        ExprPolicy::ExprDataSet exprdata;
        std::vector<ExprPolicy::ExprDataSet> datatotest;
};

int main(int argc, char** filename){
	std::string codestr = "void foo(){j = 0; \nk = 1; \ndoreme = 5; \nabc = abc + 0;\n i = j + k;\n foo(abc+doreme);}";
	std::string srcmlstr = StringToSrcML(codestr);
	std::cerr<<srcmlstr<<std::endl;
    TestExpr exprdata;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<ExprPolicy> handler {&exprdata};
    control.parse(&handler); //Start parsing
    exprdata.RunTest();
}