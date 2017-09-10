/**
 * @file TestParamTypePolicy.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <cassert>
#include <srcSAXEventDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <ParamTypePolicy.hpp>
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
	//TrimFromEnd(ch, size);
	return std::string(ch);
}

class TestParamType : public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestParamType(){}
        TestParamType(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){}
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {
            paramdata = *policy->Data<DeclData>();
            datatotest.push_back(paramdata);
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

			assert(datatotest[1].nameOfType == "Object");
			assert(datatotest[1].nameOfIdentifier == "onetwothree");
			assert(datatotest[1].linenumber == 1);
			assert(datatotest[1].isConstValue == false);
			assert(datatotest[1].isConstAlias == false);
			assert(datatotest[1].isReference == false);
			assert(datatotest[1].isPointer == false);
			assert(datatotest[1].isStatic == false);
			assert(datatotest[1].namespaces.empty());

			assert(datatotest[2].nameOfType == "Object");
			assert(datatotest[2].nameOfIdentifier == "DoReiMe");
			assert(datatotest[2].linenumber == 1);
			assert(datatotest[2].isConstValue == false);
			assert(datatotest[2].isConstAlias == false);
			assert(datatotest[2].isReference == false);
			assert(datatotest[2].isPointer == true);
			assert(datatotest[2].isStatic == true);
			assert(datatotest[2].namespaces.empty());

			assert(datatotest[3].nameOfType == "Object");
			assert(datatotest[3].nameOfIdentifier == "aybeecee");
			assert(datatotest[3].linenumber == 1);
			assert(datatotest[3].isConstValue == true);
			assert(datatotest[3].isConstAlias == true);
			assert(datatotest[3].isReference == false);
			assert(datatotest[3].isPointer == true);
			assert(datatotest[3].isStatic == false);
			assert(datatotest[3].namespaces.empty());

			assert(datatotest[4].nameOfType == "vector");
			assert(datatotest[4].nameOfIdentifier == "spaces");
			assert(datatotest[4].linenumber == 1);
			assert(datatotest[4].isConstValue == false);
			assert(datatotest[4].isConstAlias == false);
			assert(datatotest[4].isReference == false);
			assert(datatotest[4].isPointer == false);
			assert(datatotest[4].isStatic == false);
			assert(datatotest[4].namespaces.size() == 1);

			assert(datatotest[5].nameOfType == "int");
			assert(datatotest[5].nameOfIdentifier == "ab");
			assert(datatotest[5].linenumber == 1);
			assert(datatotest[5].isConstValue == false);
			assert(datatotest[5].isConstAlias == true);
			assert(datatotest[5].isReference == false);
			assert(datatotest[5].isPointer == true);
			assert(datatotest[5].isStatic == false);
			assert(datatotest[5].namespaces.empty());
			assert(datatotest[5].isClassMember == false);
			assert(datatotest[5].usesSubscript == true);			
		}
    protected:
        void * DataInner() const override {
            return (void*)0; //To silence the warning
        }
    private:

        ParamTypePolicy parampolicy;
        DeclData paramdata;
        std::vector<DeclData> datatotest;
};

int main(int argc, char** filename){
	std::string codestr = "void foo(int& abc, Object<int> onetwothree, static Object* DoReiMe, const Object* const aybeecee, std::vector<std::string> spaces, int* const ab[5]){}";
	std::string srcmlstr = StringToSrcML(codestr);

    TestParamType paramData;
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<ParamTypePolicy> handler {&paramData};
    control.parse(&handler); //Start parsing
    paramData.RunTest();
}