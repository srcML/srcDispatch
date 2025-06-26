// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testLabel.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE label tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_common_label) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { label: }", "void foo() { label: }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.ToString() == "label");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_label) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { label: }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.ToString() == "|label");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_label) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { label: }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.ToString() == "label|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_label) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { foo: }", "void foo() { bar: }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 2);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0).IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.IsDelete());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(0)->name.ToString() == "foo|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(1).IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(1)->name.IsInsert());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.at(1)->name.ToString() == "|bar");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
