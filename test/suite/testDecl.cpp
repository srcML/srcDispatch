// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testDecl.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE decl tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// templated (not arg but def)

BOOST_AUTO_TEST_CASE(decl_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a = 0;", "type a = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a = 0");
}

BOOST_AUTO_TEST_CASE(decl_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "type a = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "|type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "|a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "|a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "|0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "|type a = 0");
}

BOOST_AUTO_TEST_CASE(decl_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a = 0;", ""}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a|");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a = 0|");
}

BOOST_AUTO_TEST_CASE(name_rename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a = 0;", "type b = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsChange());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a|b");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.GetOriginal() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.GetModified() == "b");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a = 0|type b = 0");
}

BOOST_AUTO_TEST_CASE(type_rename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"foo a;", "bar a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name.IsChange());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "foo|bar");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "foo a|bar a");
}

BOOST_AUTO_TEST_CASE(type_rename_two) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"foo a;", "foo* a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 2);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name.IsCommon());

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetElement() == srcDispatch::TypeData::TypeType::POINTER);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "foo|foo *");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "foo a|foo * a");
}

BOOST_AUTO_TEST_CASE(decl_init_literal_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a = 0;", "type a = 1;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsChange());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetOriginal()->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetModified()->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetOriginal()->expr.at(0).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetModified()->expr.at(0).IsInsert());

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetOriginal()->expr.at(0).GetOriginal())->literal.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetModified()->expr.at(0).GetModified())->literal.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetOriginal()->expr.at(0).GetOriginal())->literal.ToString() == "0|");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.GetModified()->expr.at(0).GetModified())->literal.ToString() == "|1");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a = 0|type a = 1");
}

BOOST_AUTO_TEST_CASE(init_literal_change_2) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a(0);", "type a = 1;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.ToString() == "0|");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.ToString() == "0|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).GetModified())->literal.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).GetModified())->literal.ToString() == "|1");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a(0)|type a = 1");
}

BOOST_AUTO_TEST_CASE(init_literal_change_3) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a{0};", "type a = 1;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.ToString() == "0|");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.ToString() == "0|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).GetModified())->literal.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).GetModified())->literal.ToString() == "|1");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a(0)|type a = 1");
}

BOOST_AUTO_TEST_CASE(name_only_match) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type* a(0);", "typePtr a = 1;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 2);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name.IsChange());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "type|typePtr");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetElement() == srcDispatch::TypeData::TypeType::POINTER);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type *|typePtr");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.ToString() == "0|");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->arguments.at(0)->expr.at(0).GetOriginal())->literal.ToString() == "0|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).GetModified())->literal.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.at(0).GetModified())->literal.ToString() == "|1");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type * a(0)|typePtr a = 1");
}

BOOST_AUTO_TEST_CASE(comma_separated_decl) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a = 0, b = 1, c = 2;", "type a = 0, c = 2, d = 3;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 4);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1).IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->generics.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->name.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->name->name.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->name.ToString() == "b|");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->name->name.ToString() == "b|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->init.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1)->init.ToString() == "1|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->generics.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->name.ToString() == "c");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->name->name.ToString() == "c");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2)->init.ToString() == "2");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3).IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->name.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->name->name.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->name.ToString() == "|d");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->name->name.ToString() == "|d");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->init.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3)->init.ToString() == "|3");

    // a little weird as type is shared, so always in common
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).ToString() == "type a = 0");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(1).ToString() == "type b = 1|type");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(2).ToString() == "type c = 2");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(3).ToString() == "type|type d = 3");
}

// compound name
BOOST_AUTO_TEST_CASE(decl_compound_name_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"std::string str = \"\";", "std::string str = \"\";"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 3);
    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first).IsCommon());

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->names.size() == 0);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->name.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->name.ToString() == "std");

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::OperatorData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1).GetElement())->op.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::OperatorData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1).GetElement())->op.ToString() == "::");

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->names.size() == 0);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->name.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->name.ToString() == "string");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "std::string");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "std::string");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size()    == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString()       == "str");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "str");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "\"\"");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "std::string str = \"\"");
}

BOOST_AUTO_TEST_CASE(decl_compound_name_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"std::string str = \"\";", "std::wstring str = \"\";"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 3);
    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first).IsCommon());

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->names.size() == 0);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->name.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->name.ToString() == "std");

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::OperatorData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1).GetElement())->op.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::OperatorData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1).GetElement())->op.ToString() == "::");

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->names.size() == 0);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->name.IsChange());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->name.ToString() == "string|wstring");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "std::string|std::wstring");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "std::string|std::wstring");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size()    == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString()       == "str");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "str");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "\"\"");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "std::string str = \"\"|std::wstring str = \"\"");
}

BOOST_AUTO_TEST_CASE(decl_simple_to_complex_name) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"string str = \"\";", "std::string str = \"\";"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsInsert());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 3);
    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first).IsInsert());

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0)).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->names.size() == 0);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->name.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement())->name.ToString() == "|std");

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1)).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::OperatorData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1).GetElement())->op.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::OperatorData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(1).GetElement())->op.ToString() == "|::");

    BOOST_TEST(std::any_cast<srcDispatch::DeltaElement<std::any>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2)).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->names.size() == 0);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->name.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
                    runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(2).GetElement())->name.ToString() == "string");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "string|std::string");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "string|std::string");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size()    == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString()       == "str");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "str");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "\"\"");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "string str = \"\"|std::string str = \"\"");
}

// specifier test
BOOST_AUTO_TEST_CASE(decl_add_specifier) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a = 0;", "const type a = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 2);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::SPECIFIER);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<std::string>>() == "|const");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type|const type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a = 0|const type a = 0");
}

// specifier test
BOOST_AUTO_TEST_CASE(decl_remove_specifier) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"const type a = 0;", "type a = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 2);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::SPECIFIER);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<std::string>>() == "const|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "const type|type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "const type a = 0|type a = 0");
}

// specifier test
BOOST_AUTO_TEST_CASE(decl_change_specifier) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"static type a = 0;", "const type a = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 3);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::SPECIFIER);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<std::string>>() == "static|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetElement() == srcDispatch::TypeData::TypeType::SPECIFIER);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).first.ToString<std::shared_ptr<std::string>>() == "|const");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(2).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(2).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(2).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(2).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "static type|const type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "static type a = 0|const type a = 0");
}

// modifier change test
BOOST_AUTO_TEST_CASE(decl_modifier_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type* a;", "type& a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 2);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.ToString<std::shared_ptr<srcDispatch::NameData>>() == "type");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.IsChange());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetOriginal() == srcDispatch::TypeData::TypeType::POINTER);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(1).second.GetModified() == srcDispatch::TypeData::TypeType::REFERENCE);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type *|type &");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type * a|type & a");
}

// templated name
BOOST_AUTO_TEST_CASE(decl_template_argument_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type<arg> a;", "type<arg> a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.IsCommon()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.ToString() == "type"
    );

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).ToString() == "arg");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type<arg>");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type<arg> a");
}

BOOST_AUTO_TEST_CASE(decl_template_argument_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type a;", "type<arg> a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsInsert());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsInsert());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.IsCommon()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.ToString() == "type"
    );

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).ToString() == "|arg");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type|type<arg>");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a|type<arg> a");
}

BOOST_AUTO_TEST_CASE(decl_template_argument_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type<arg> a;", "type a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsDelete());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsDelete());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.IsCommon()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.ToString() == "type"
    );

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).ToString() == "arg|");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type<arg>|type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type<arg> a|type a");
}

BOOST_AUTO_TEST_CASE(decl_template_argument_insert_arg) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type<foo> a;", "type<foo, bar> a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.IsCommon()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.ToString() == "type"
    );

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.size() == 2);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).ToString() == "foo");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(1).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(1).ToString() == "|bar");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type<foo>|type<foo, bar>");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type<foo> a|type<foo, bar> a");
}

BOOST_AUTO_TEST_CASE(decl_template_argument_delete_arg) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type<foo, bar> a;", "type<bar> a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.IsCommon()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.ToString() == "type"
    );

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.size() == 2);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).ToString() == "foo|");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(1).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(1).ToString() == "bar");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type<foo, bar>|type<bar>");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type<foo, bar> a|type<bar> a");
}

BOOST_AUTO_TEST_CASE(decl_template_argument_change_arg) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"type<foo> a;", "type<bar> a;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 0);
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());

    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->name);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.IsCommon()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->names.at(0).GetElement()
        )->name.ToString() == "type"
    );

    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0)->expr.size() == 1);
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0)->expr.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
            runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0)->expr.at(0).GetElement()
        )->names.size() == 0
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
            runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0)->expr.at(0).GetElement()
        )->name.IsChange()
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
            runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0)->expr.at(0).GetElement()
        )->name.GetOriginal() == "foo"
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
        std::any_cast<std::shared_ptr<srcDispatch::NameData>>(
            runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0)->expr.at(0).GetElement()
        )->name.GetModified() == "bar"
    );
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.GetElement())->templateArgumentList->arguments.at(0).ToString() == "foo|bar");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type<foo>|type<bar>");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type<foo> a|type<bar> a");
}

// templated
BOOST_AUTO_TEST_CASE(decl_template_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"template<typename type> type a = 0;", "template<typename type> type a = 0;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.at(0).IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->generics.at(0).ToString() == "template<typename type>");
    BOOST_TEST(!runner.GetDeclStmtInfo().at(0)->decls.at(0)->accessSpecifier);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->type.ToString() == "type");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name.ToString() == "a");
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->name->name.ToString() == "a");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.IsCommon());
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().at(0)->decls.at(0)->init.ToString() == "0");

    BOOST_TEST(runner.GetDeclStmtInfo().at(0).ToString() == "type a = 0");
}
