#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <CollectNLContext.hpp>
#include <cassert>
#include <srcml.h>
#include <string>
#include <fstream>
#include <streambuf>

std::string filetostring (const std::string file){
	std::ifstream t(file);
	std::string str;
	
	t.seekg(0, std::ios::end); 
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	
	str.assign((std::istreambuf_iterator<char>(t)),
            	std::istreambuf_iterator<char>());
	return str;
}
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
	ch[size] = 0;
	return std::string(ch);
}

class TestSNLPolicy : public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestSNLPolicy(){}
        TestSNLPolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){}
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            sourcenlpdata = *policy->Data<NLContextPolicy::NLContextData>();
            for(auto data : sourcenlpdata.nlsetmap){
            	std::cerr<<data.name<<","<<data.stereotype<<","<<data.context<<","<<data.category<<std::endl;
            }
        }
    protected:
        void * DataInner() const override {
            return (void*)0; //To silence the warning
        }
    private:
        NLContextPolicy::NLContextData sourcenlpdata;
};

int main(int argc, char** filename){
	std::string srcmlstr = filetostring(filename[1]);

    TestSNLPolicy decltypedata;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<NLContextPolicy> handler {&decltypedata};
    control.parse(&handler); //Start parsing
}