// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testClass.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE class_and_struct tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// Do I need to collect class decl?

BOOST_AUTO_TEST_CASE(class_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo {};", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

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

BOOST_AUTO_TEST_CASE(struct_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"struct foo {};", "struct bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::STRUCT);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsInsert());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsInsert());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsInsert());
    BOOST_TEST(classData->name.ToString() == "|foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo {};", ""}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsDelete());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsDelete());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsDelete());
    BOOST_TEST(classData->name.ToString() == "foo|");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_rename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo {};", "class bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// parents
BOOST_AUTO_TEST_CASE(class_parent_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : bar {};", "class foo : bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0). IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo {};", "class foo : bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsInsert());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsInsert());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "|bar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : bar {};", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsDelete());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsDelete());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar|");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_rename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : public bar {};", "class foo : public foobar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar|foobar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(bool(classData->parents.at(0)->accessSpecifier));
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.IsCommon());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_common_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : public bar {};", "class foo : public bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(bool(classData->parents.at(0)->accessSpecifier));
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_insert_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : bar {};", "class foo : public bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(bool(classData->parents.at(0)->accessSpecifier));
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_delete_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : public bar {};", "class foo : bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(bool(classData->parents.at(0)->accessSpecifier));
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_change_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : public bar {};", "class foo : private bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(bool(classData->parents.at(0)->accessSpecifier));
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.IsChange());
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.GetOriginal() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->parents.at(0)->accessSpecifier.GetModified() == AccessSpecifier::PRIVATE);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_common_virtual) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : virtual bar {};", "class foo : virtual bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(bool(classData->parents.at(0)->isVirtual));
    BOOST_TEST(classData->parents.at(0)->isVirtual.IsCommon());
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_insert_virtual) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : bar {};", "class foo : virtual bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(bool(classData->parents.at(0)->isVirtual));
    BOOST_TEST(classData->parents.at(0)->isVirtual.IsInsert());
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_delete_virtual) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : virtual bar {};", "class foo : bar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 1);
    BOOST_TEST(classData->parents.at(0).IsCommon());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsCommon());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar");
    BOOST_TEST(bool(classData->parents.at(0)->isVirtual));
    BOOST_TEST(classData->parents.at(0)->isVirtual.IsDelete());
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_parent_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo : bar {};", "class foo : foobar {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.size() == 2);

    BOOST_TEST(classData->parents.at(0).IsDelete());
    BOOST_TEST(classData->parents.at(0)->name);
    BOOST_TEST(classData->parents.at(0)->name.IsDelete());
    BOOST_TEST(classData->parents.at(0)->name.ToString() == "bar|");
    BOOST_TEST(!classData->parents.at(0)->isVirtual);
    BOOST_TEST(!classData->parents.at(0)->accessSpecifier);

    BOOST_TEST(classData->parents.at(1).IsInsert());
    BOOST_TEST(classData->parents.at(1)->name);
    BOOST_TEST(classData->parents.at(1)->name.IsInsert());
    BOOST_TEST(classData->parents.at(1)->name.ToString() == "|foobar");
    BOOST_TEST(!classData->parents.at(1)->isVirtual);
    BOOST_TEST(!classData->parents.at(1)->accessSpecifier);

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// fields
BOOST_AUTO_TEST_CASE(class_fields_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { int i; };", "class foo { int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_private) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: int i; };", "class foo { private: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_private_default) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { int i; };", "class foo { private: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_public) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: int i; };", "class foo { public: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_protected) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { protected: int i; };", "class foo { protected: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_access_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: int i; };", "class foo { protected: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsChange());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetOriginal() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetModified() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: };", "class foo { private: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsInsert());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsInsert());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "|int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_insert_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: };", "class foo { private: int i; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsInsert());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsInsert());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "|int i");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: int i; };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i|");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_delete_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: int i; };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 1);
    BOOST_TEST(classData->fields.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i|");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: int i; };", "class foo { private: double d; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 2);
    BOOST_TEST(classData->fields.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i|");

    BOOST_TEST(classData->fields.at(1).IsInsert());
    BOOST_TEST(classData->fields.at(1)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(1)->decls.at(0).IsInsert());
    BOOST_TEST(classData->fields.at(1)->decls.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->fields.at(1)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(1).ToString() == "|double d");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_fields_replace_with_namespace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: int i; };", "class foo { private: double d; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.size() == 2);
    BOOST_TEST(classData->fields.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(0)->decls.at(0).IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->fields.at(0)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->fields.at(0).ToString() == "int i|");

    BOOST_TEST(classData->fields.at(1).IsInsert());
    BOOST_TEST(classData->fields.at(1)->decls.size() == 1);
    BOOST_TEST(classData->fields.at(1)->decls.at(0).IsInsert());
    BOOST_TEST(classData->fields.at(1)->decls.at(0)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->fields.at(1)->decls.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->fields.at(1).ToString() == "|double d");

    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// constructor
BOOST_AUTO_TEST_CASE(class_constructors_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { foo() {} };", "class foo { foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_private) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: foo() {} };", "class foo { private: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_private_default) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { foo() {} };", "class foo { private: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_public) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: foo() {} };", "class foo { public: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_protected) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { protected: foo() {} };", "class foo { protected: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_access_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: foo() {} };", "class foo { protected: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsChange());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetOriginal() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetModified() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: };", "class foo { private: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsInsert());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "|foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_insert_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: };", "class foo { private: foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsInsert());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "|foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: foo() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsDelete());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_delete_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: foo() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 1);
    BOOST_TEST(classData->constructors.at(0).IsDelete());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: foo() { ab; } public: void f() { c; d; } };", "class bar { private: bar() { b; } public: void f() { c; d; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 2);
    BOOST_TEST(classData->constructors.at(0).IsDelete());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}|");

    BOOST_TEST(classData->constructors.at(1).IsInsert());
    BOOST_TEST(classData->constructors.at(1)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->constructors.at(1)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(1).ToString() == "|bar() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_constructors_replace_with_namespace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: foo() { a; } public: void f() { c + d + e + f; } };", "class bar { private: bar() { a; } public: void f() { c + d + e + f; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->constructors.size() == 2);
    BOOST_TEST(classData->constructors.at(0).IsDelete());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->constructors.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->constructors.at(0).ToString() == "foo() {}|");

    BOOST_TEST(classData->constructors.at(1).IsInsert());
    BOOST_TEST(classData->constructors.at(1)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->constructors.at(1)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->constructors.at(1).ToString() == "|bar() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// destructor
BOOST_AUTO_TEST_CASE(class_destructors_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { ~foo() {} };", "class foo { ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_private) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: ~foo() {} };", "class foo { private: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_private_default) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { ~foo() {} };", "class foo { private: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_public) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: ~foo() {} };", "class foo { public: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_protected) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { protected: ~foo() {} };", "class foo { protected: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_access_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: ~foo() {} };", "class foo { protected: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.IsChange());
    BOOST_TEST(classData->destructor->accessSpecifier.GetOriginal() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor->accessSpecifier.GetModified() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: };", "class foo { private: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsInsert());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "|~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_insert_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: };", "class foo { private: ~foo() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsInsert());
    BOOST_TEST(classData->destructor->accessSpecifier.IsInsert());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "|~foo() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: ~foo() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsDelete());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_delete_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: ~foo() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsDelete());
    BOOST_TEST(classData->destructor->accessSpecifier.IsDelete());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: ~foo() { ab; } public: void f() { c; d; } };", "class bar { private: ~bar() { b; } public: void f() { c; d; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsChange());
    BOOST_TEST(classData->destructor->accessSpecifier.IsCommon());
    BOOST_TEST(classData->destructor->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}|~bar() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_replace_with_namespace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: ~foo() { a; } public: void f() { c + d + e + f; } };", "class bar { private: ~bar() { a; } public: void f() { c + d + e + f; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsChange());
    BOOST_TEST(classData->destructor.GetOriginal()->accessSpecifier.GetOriginal() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->destructor.GetModified()->accessSpecifier.GetModified() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}|~bar() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_destructors_replace_with_convert_namespace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: ~foo() {} void f(int c, double d, long e, float f) { c + d + e + f; } };", "class bar { private: ~bar() { delete a; } void f(int c, double d, long e, float f) { c + d + e + f; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo|bar");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->destructor);
    BOOST_TEST(classData->destructor.IsChange());
    BOOST_TEST(classData->destructor->accessSpecifier.IsChange());
    BOOST_TEST(classData->destructor->accessSpecifier.GetOriginal() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->destructor->accessSpecifier.GetModified() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->destructor.ToString() == "~foo() {}|~bar() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// operators
BOOST_AUTO_TEST_CASE(class_operators_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { bool operator==() {} };", "class foo { bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_private) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: bool operator==() {} };", "class foo { private: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_private_default) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { bool operator==() {} };", "class foo { private: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_public) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: bool operator==() {} };", "class foo { public: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_protected) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { protected: bool operator==() {} };", "class foo { protected: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_access_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: bool operator==() {} };", "class foo { protected: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsChange());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetOriginal() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetModified() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: };", "class foo { private: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsInsert());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "|bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_insert_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: };", "class foo { private: bool operator==() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsInsert());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "|bool operator==() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: bool operator==() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsDelete());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_delete_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: bool operator==() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 1);
    BOOST_TEST(classData->operators.at(0).IsDelete());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: bool operator==(int b) { return std::max(b - a, 0); } };", "class foo { private: bool operator!=(const object& that) { return *this != that; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 2);
    BOOST_TEST(classData->operators.at(0).IsDelete());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==(int b) {}|");

    BOOST_TEST(classData->operators.at(1).IsInsert());
    BOOST_TEST(classData->operators.at(1)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->operators.at(1)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(1).ToString() == "|bool operator!=(const object & that) {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_operators_replace_with_namespace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: bool operator==(int b) { return std::max(b - a, 0); } };", "class foo { private: bool operator!=(const object& that) { return *this != that; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->operators.size() == 2);
    BOOST_TEST(classData->operators.at(0).IsDelete());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->operators.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->operators.at(0).ToString() == "bool operator==(int b) {}|");

    BOOST_TEST(classData->operators.at(1).IsInsert());
    BOOST_TEST(classData->operators.at(1)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->operators.at(1)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->operators.at(1).ToString() == "|bool operator!=(const object & that) {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->methods.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// methods
BOOST_AUTO_TEST_CASE(class_methods_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { void f() {} };", "class foo { void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_private) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: void f() {} };", "class foo { private: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_private_default) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { void f() {} };", "class foo { private: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_public) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: void f() {} };", "class foo { public: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_protected) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { protected: void f() {} };", "class foo { protected: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_access_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: void f() {} };", "class foo { protected: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsChange());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetOriginal() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetModified() == AccessSpecifier::PROTECTED);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: };", "class foo { private: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsInsert());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "|void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_insert_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: };", "class foo { private: void f() {} };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsInsert());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "|void f() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: void f() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsDelete());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_delete_with_access) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: void f() {} };", "class foo { private: };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->methods.at(0).IsDelete());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}|");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { private: void f() { a; } };", "class foo { private: int g() { b; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 2);
    BOOST_TEST(classData->methods.at(0).IsDelete());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}|");

    BOOST_TEST(classData->methods.at(1).IsInsert());
    BOOST_TEST(classData->methods.at(1)->accessSpecifier.IsCommon());
    BOOST_TEST(classData->methods.at(1)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(1).ToString() == "|int g() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

BOOST_AUTO_TEST_CASE(class_methods_replace_with_namespace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { public: void f() { a; } };", "class foo { private: int g() { b; } };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->methods.size() == 2);
    BOOST_TEST(classData->methods.at(0).IsDelete());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.IsDelete());
    BOOST_TEST(classData->methods.at(0)->accessSpecifier.GetElement() == AccessSpecifier::PUBLIC);
    BOOST_TEST(classData->methods.at(0).ToString() == "void f() {}|");

    BOOST_TEST(classData->methods.at(1).IsInsert());
    BOOST_TEST(classData->methods.at(1)->accessSpecifier.IsInsert());
    BOOST_TEST(classData->methods.at(1)->accessSpecifier.GetElement() == AccessSpecifier::PRIVATE);
    BOOST_TEST(classData->methods.at(1).ToString() == "|int g() {}");

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(!classData->isAbstract);
}

// abstract
BOOST_AUTO_TEST_CASE(class_is_abstract) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { virtual void f() = 0; };", "class foo { virtual void f() = 0; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(classData->isAbstract);
    BOOST_TEST(classData->isAbstract.IsCommon());
    BOOST_TEST(classData->isAbstract.GetElement());
}

BOOST_AUTO_TEST_CASE(class_becomes_abstract) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { };", "class foo { virtual void f() = 0; };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(classData->isAbstract);
    BOOST_TEST(classData->isAbstract.IsInsert());
    BOOST_TEST(classData->isAbstract.GetElement());
}

BOOST_AUTO_TEST_CASE(class_was_abstract) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo { virtual void f() = 0; };", "class foo { };"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.empty());
    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

    BOOST_TEST(classData->name.IsCommon());
    BOOST_TEST(classData->name.ToString() == "foo");

    BOOST_TEST(classData->parents.empty());

    BOOST_TEST(classData->fields.empty());
    BOOST_TEST(classData->constructors.empty());
    BOOST_TEST(!classData->destructor);
    BOOST_TEST(classData->operators.empty());
    BOOST_TEST(classData->methods.size() == 1);
    BOOST_TEST(classData->innerClasses.empty());

    BOOST_TEST(classData->isAbstract);
    BOOST_TEST(classData->isAbstract.IsDelete());
    BOOST_TEST(classData->isAbstract.GetElement());
}

// generic
BOOST_AUTO_TEST_CASE(class_is_generic) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"template<typename type> class foo {};", "template<typename type> class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.size() == 1);
    BOOST_TEST(classData->generics.at(0).IsCommon());
    BOOST_TEST(classData->generics.at(0).ToString() == "template<typename type>");

    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

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

BOOST_AUTO_TEST_CASE(class_becomes_generic) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo {};", "template<typename type> class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.size() == 1);
    BOOST_TEST(classData->generics.at(0).IsInsert());
    BOOST_TEST(classData->generics.at(0).ToString() == "|template<typename type>");

    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

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

BOOST_AUTO_TEST_CASE(class_was_generic) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"template<typename type> class foo {};", "class foo {};"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classData = runner.GetClassInfo().at(0);
    BOOST_TEST(classData.IsCommon());

    BOOST_TEST(classData->generics.size() == 1);
    BOOST_TEST(classData->generics.at(0).IsDelete());
    BOOST_TEST(classData->generics.at(0).ToString() == "template<typename type>|");

    BOOST_TEST(classData->type.IsCommon());
    BOOST_TEST(classData->type.GetElement() == srcDispatch::ClassData::CLASS);

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

// Inner Classes
