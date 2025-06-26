// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testFunction.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE template tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(template_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "template<typename type> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_insert_front) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename foo> void func() {}", "template<typename bar> template<typename foo> void func() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 2);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "|template<typename bar>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).ToString() == "template<typename foo>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void func() {}");
}

BOOST_AUTO_TEST_CASE(template_insert_back) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename foo> void func() {}", "template<typename foo> template<typename bar> void func() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 2);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename foo>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).ToString() == "|template<typename bar>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void func() {}");
}

BOOST_AUTO_TEST_CASE(template_delete_front) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename bar> template<typename foo> void func() {}", "template<typename foo> void func() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 2);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename bar>|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).ToString() == "template<typename foo>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void func() {}");
}

BOOST_AUTO_TEST_CASE(template_delete_back) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename foo> template<typename bar> void func() {}", "template<typename foo> void func() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 2);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename foo>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(1).ToString() == "template<typename bar>|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void func() {}");
}

BOOST_AUTO_TEST_CASE(template_insert_parameter_front) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "template<class bar, typename type> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(1).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>|template<class bar, typename type>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_insert_parameter_back) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "template<typename type, class bar> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(1).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>|template<typename type, class bar>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_delete_parameter_front) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<class bar, typename type> void foo() {}", "template<typename type> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(1).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<class bar, typename type>|template<typename type>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_delete_parameter_back) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type, class bar> void foo() {}", "template<typename type> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(1).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type, class bar>|template<typename type>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_modify_parameter_type) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "template<class type> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->type.ToString() == "typename|class");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->name.ToString() == "type");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).ToString() == "typename type|class type");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>|template<class type>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_modify_parameter_name) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "template<typename T> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->type.ToString() == "typename");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->name.ToString() == "type|T");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).ToString() == "typename type|typename T");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>|template<typename T>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_modify_parameter_init) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "template<typename type = object> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->type.ToString() == "typename");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0)->name.ToString() == "type");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0)->parameters.at(0).ToString() == "typename type|typename type = object");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>|template<typename type = object>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_common_int) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<int num> void foo() {}", "template<int num> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<int num>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(template_common_int_insert_init) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<int num> void foo() {}", "template<int num = 0> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<int num>|template<int num = 0>");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
