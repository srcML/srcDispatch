// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testFunction.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE function tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(function_foo) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("#include <iostream>\n#include <string>\n\nclass Person {\nprivate:\n    std::string name_;\n    int age_;\n\npublic:\n    // Constructor\n    Person(const std::string& NAME, int AGE): name_(NAME), age_(AGE) {}\n\n    // Getter\n    std::string getName() const {\n        return name_;\n    }\n\n    // Setter\n    void setName(const std::string& newName) {\n        name_ = newName;\n    }\n\n    // Method\n    void sayHello() const {\n        std::cout << \"Hi, my name is \" << name_ << \" and I am \" << age_ << \" years old.\n\";\n   }\n", "#include <iostream>\n#include <string>\n\nclass Person {\nprivate:\n    std::string name_;\n    int age_;\n\npublic:\n    // Constructor\n    Person(const std::string& NAME, int AGE): name_(NAME), age_(AGE) {}\n\n    // Getter\n    std::string getName() const {\n        return name_;\n    }\n\n    // Setter\n    void setName(const std::string& newName) {\n        name_ = newName;\n    }\n\n    // Method\n    void sayHello() const {\n        std::cout << \"Hi, my name is \" << name_ << \" and I am \" << age_ << \" years old.\n\";\n   }\n");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    std::cerr << "HERE: " << __FILE__ << ' ' << __FUNCTION__ << ' ' << __LINE__ << ' ' << runner.GetClassInfo().at(0)->constructors.at(0) << '\n';
}

BOOST_AUTO_TEST_CASE(function_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(function_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "|void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "|foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "|foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "|void foo() {}");
}

BOOST_AUTO_TEST_CASE(function_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}|");
}

BOOST_AUTO_TEST_CASE(function_name_change) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void bar() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsChange());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo|bar");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.GetOriginal() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.GetModified() == "bar");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}|void bar() {}");
}

BOOST_AUTO_TEST_CASE(function_return_type_change) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "int foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.GetElement())->name.IsChange());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void|int");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}|int foo() {}");
}

BOOST_AUTO_TEST_CASE(function_add_parameter) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo(int i) {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).first.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.ToString() == "|int");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.ToString() == "|i");
    
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}|void foo(int i) {}");
}

BOOST_AUTO_TEST_CASE(function_remove_parameter) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo(double d) {}", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.ToString() == "double|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.ToString() == "d|");
    
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo(double d) {}|void foo() {}");
}

BOOST_AUTO_TEST_CASE(function_parameter_type_change) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo(double d) {}", "void foo(int d) {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).first.GetElement())->name.IsChange());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.ToString() == "double|int");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.ToString() == "d");
    
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo(double d) {}|void foo(int d) {}");
}

BOOST_AUTO_TEST_CASE(function_parameter_name_change) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo(double d) {}", "void foo(double num) {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.ToString() == "double");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name->name.IsChange());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.ToString() == "d|num");
    
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo(double d) {}|void foo(double num) {}");
}

BOOST_AUTO_TEST_CASE(function_parameter_multi_change) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo(int i, double d) {}", "void foo(double d, long l) {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 3);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->type.ToString() == "int|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(0)->name.ToString() == "i|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(1).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(1)->type.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(1)->type->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(runner.GetFunctionInfo().at(0)->parameters.at(1)->type->types.at(0).first.GetElement())->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(1)->type.ToString() == "double");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(1)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(1)->name.ToString() == "d");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->type.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->type->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->type->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->type->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->type->types.at(0).first.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->type.ToString() == "|long");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.at(2)->name.ToString() == "|l");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo(int i, double d) {}|void foo(double d, long l) {}");
}

// decl to def
BOOST_AUTO_TEST_CASE(function_decl_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo();", "void foo();");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo();");
}

// specifiers
BOOST_AUTO_TEST_CASE(function_specifier_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo();", "void foo() const;");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).ToString() == "|const");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo();|void foo() const;");
}

BOOST_AUTO_TEST_CASE(function_specifier_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() const;", "void foo();");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).ToString() == "const|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() const;|void foo();");
}

BOOST_AUTO_TEST_CASE(function_pure_virtual_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo();", "void foo() = 0;");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isPureVirtual.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isPureVirtual.GetElement());

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);


    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo();|void foo() = 0;");
}

BOOST_AUTO_TEST_CASE(function_pure_virtual_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() = 0;", "void foo();");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isPureVirtual.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isPureVirtual.GetElement());

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);


    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() = 0;|void foo();");
}

BOOST_AUTO_TEST_CASE(function_pure_delete_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo();", "void foo() = delete;");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->isDelete);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isDelete.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isDelete.GetElement());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo();|void foo() = delete;");
}

BOOST_AUTO_TEST_CASE(function_pure_delete_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() = delete;", "void foo();");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isDelete);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isDelete.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->isDelete.GetElement());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() = delete;|void foo();");
}

// block
BOOST_AUTO_TEST_CASE(function_block_insert_expr_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { a; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::ExprStmtData& expr = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(expr.expr);
    BOOST_TEST(expr.expr.IsInsert());
    BOOST_TEST(expr.expr->expr.size() == 1);
    BOOST_TEST(expr.expr->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(expr.expr->expr.at(0).GetElement())->name.IsInsert());
    BOOST_TEST(expr.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "|a");
    BOOST_TEST(expr.expr.ToString() == "|a");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "|a");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()        == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size()       == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

// operator
BOOST_AUTO_TEST_CASE(function_operator_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("bool operator==() {}", "bool operator==() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::OPERATOR);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "bool");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "operator==");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "operator");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "bool operator==() {}");
}

BOOST_AUTO_TEST_CASE(function_operator_common_decl) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("bool operator==();", "bool operator==();");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::OPERATOR);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "bool");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "operator==");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "operator");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "bool operator==();");
}

BOOST_AUTO_TEST_CASE(function_operator_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("", "bool operator==() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::OPERATOR);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "|bool");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "|operator==");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "|operator");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "|bool operator==() {}");
}

BOOST_AUTO_TEST_CASE(function_operator_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("bool operator==() {}", "");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::OPERATOR);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "bool|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "operator==|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "operator|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "bool operator==() {}|");
}

BOOST_AUTO_TEST_CASE(function_operator_type) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("operator bool() {}", "operator bool() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::OPERATOR);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->returnType);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "operator bool");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "operator ");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "operator bool() {}");
}

// template function
BOOST_AUTO_TEST_CASE(function_template_common) {

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

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(function_template_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "template<typename type> void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "|template<typename type>");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(function_template_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("template<typename type> void foo() {}", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.at(0).ToString() == "template<typename type>|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDiffDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDiffDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

// decl to defn (add to srcDiff first)
