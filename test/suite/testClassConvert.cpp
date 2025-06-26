// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testClassConvert.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE class type convert tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// Do I need to collect class decl?

BOOST_AUTO_TEST_CASE(class_to_struct) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("class foo {};", "struct foo {};");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsChange());
    BOOST_TEST(classData->type.GetOriginal() == srcDispatch::ClassData::CLASS);
    BOOST_TEST(classData->type.GetModified() == srcDispatch::ClassData::STRUCT);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(struct_to_class) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("struct foo {};", "class foo {};");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsChange());
    BOOST_TEST(classData->type.GetOriginal() == srcDispatch::ClassData::STRUCT);
    BOOST_TEST(classData->type.GetModified() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// inner/local class convert
