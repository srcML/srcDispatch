// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testFunction.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
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

BOOST_AUTO_TEST_CASE(function_trailing_specifier_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() const;", "void foo() const;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).ToString() == "const");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() const;");
}

BOOST_AUTO_TEST_CASE(function_trailing_specifier_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo();", "void foo() const;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDispatch::FunctionData::FUNCTION);

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
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
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

BOOST_AUTO_TEST_CASE(function_trailing_specifier_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() const;", "void foo();"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDispatch::FunctionData::FUNCTION);

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
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
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

BOOST_AUTO_TEST_CASE(function_leading_specifier_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"virtual void foo();", "virtual void foo();"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.at(0).ToString() == "virtual");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "virtual void foo();");
}

BOOST_AUTO_TEST_CASE(function_leading_specifier_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo();", "virtual void foo();"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.at(0).ToString() == "|virtual");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo();|virtual void foo();");
}

BOOST_AUTO_TEST_CASE(function_leading_specifier_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"virtual void foo();", "void foo();"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->generics.empty());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->type.GetElement() == srcDispatch::FunctionData::FUNCTION);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->accessSpecifier);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isPureVirtual);
    BOOST_TEST(!runner.GetFunctionInfo().at(0)->isDelete);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.size()  == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->leadingSpecifiers.at(0).ToString() == "virtual|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->trailingSpecifiers.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).second.GetElement() == srcDispatch::TypeData::TypeType::TYPENAME);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType->types.at(0).first.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->returnType.ToString() == "void");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->names.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name.ToString() == "foo");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->name->name.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->parameters.size() == 0);

    BOOST_TEST(!runner.GetFunctionInfo().at(0)->block);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "virtual void foo();|void foo();");
}
