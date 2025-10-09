// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testClass.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE class specifier tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(class_access_common_none) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"class foo {};", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::NONE);
    
    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_access_common_public) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"public class foo {};", "public class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    
    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_access_common_private) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"private class foo {};", "private class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    
    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_access_common_protected) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"protected class foo {};", "protected class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::PROTECTED);
    
    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_access_insert) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"class foo {};", "public class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsInsert());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    
    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_access_delete) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"public class foo {};", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsDelete());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    
    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_access_change) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"private class foo {};", "public class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsChange());
    BOOST_TEST(classData->accessSpecifier.GetOriginal() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->accessSpecifier.GetModified() == AccessSpecifier::PUBLIC);

    BOOST_TEST(classData->specifiers.size() == 0);

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

BOOST_AUTO_TEST_CASE(class_specifier_common) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"abstract class foo {};", "abstract class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::NONE);

    BOOST_TEST(classData->specifiers.size() == 1);
    BOOST_TEST(classData->specifiers.at(0).IsCommon());
    BOOST_TEST(*classData->specifiers.at(0).GetElement() == "abstract");

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


BOOST_AUTO_TEST_CASE(class_specifier_multi_common) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"final abstract class foo {};", "final abstract class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::NONE);

    BOOST_TEST(classData->specifiers.size() == 2);
    BOOST_TEST(classData->specifiers.at(0).IsCommon());
    BOOST_TEST(*classData->specifiers.at(0).GetElement() == "final");

    BOOST_TEST(classData->specifiers.at(1).IsCommon());
    BOOST_TEST(*classData->specifiers.at(1).GetElement() == "abstract");

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

BOOST_AUTO_TEST_CASE(class_specifier_insert) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"class foo {};", "final class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::NONE);

    BOOST_TEST(classData->specifiers.size() == 1);
    BOOST_TEST(classData->specifiers.at(0).IsInsert());
    BOOST_TEST(*classData->specifiers.at(0).GetElement() == "final");

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

BOOST_AUTO_TEST_CASE(class_specifier_delete) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"abstract class foo {};", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::NONE);

    BOOST_TEST(classData->specifiers.size() == 1);
    BOOST_TEST(classData->specifiers.at(0).IsDelete());
    BOOST_TEST(*classData->specifiers.at(0).GetElement() == "abstract");

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

BOOST_AUTO_TEST_CASE(class_specifier_change) {

    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"abstract class foo {};", "final class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->accessSpecifier.IsCommon());
    BOOST_TEST(classData->accessSpecifier.GetElement() == AccessSpecifier::NONE);

    BOOST_TEST(classData->specifiers.size() == 2);
    BOOST_TEST(classData->specifiers.at(0).IsDelete());
    BOOST_TEST(*classData->specifiers.at(0).GetElement() == "abstract");

    BOOST_TEST(classData->specifiers.at(1).IsInsert());
    BOOST_TEST(*classData->specifiers.at(1).GetElement() == "final");

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
