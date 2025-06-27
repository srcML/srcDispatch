// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testCase.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE case_expr tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <SwitchPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_common_case) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { switch(1) { case foo; } }", "void foo() { switch(1) { case foo: }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.block.IsCommon());
    BOOST_TEST(switchData.block->statements.size() == 0);

    BOOST_TEST(switchData.block->cases.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0).IsCommon());
    BOOST_TEST(switchData.block->cases.at(0)->expr.IsCommon());
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.at(0).IsCommon());
    BOOST_TEST(switchData.block->cases.at(0).ToString() == "foo");

    BOOST_TEST(switchData.block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_case) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { switch(1) {} }", "void foo() { switch(1) { case foo: }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.block.IsCommon());
    BOOST_TEST(switchData.block->statements.size() == 0);

    BOOST_TEST(switchData.block->cases.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0).IsInsert());
    BOOST_TEST(switchData.block->cases.at(0)->expr.IsInsert());
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.at(0).IsInsert());
    BOOST_TEST(switchData.block->cases.at(0).ToString() == "|foo");

    BOOST_TEST(switchData.block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_case) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { switch(1) { case foo: } }", "void foo() { switch(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.block.IsCommon());
    BOOST_TEST(switchData.block->statements.size() == 0);

    BOOST_TEST(switchData.block->cases.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0).IsDelete());
    BOOST_TEST(switchData.block->cases.at(0)->expr.IsDelete());
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.at(0).IsDelete());
    BOOST_TEST(switchData.block->cases.at(0).ToString() == "foo|");

    BOOST_TEST(switchData.block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_case_rename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { switch(1) { case foo: } }", "void foo() { switch(1) { case bar: } }"}});

   BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.block.IsCommon());
    BOOST_TEST(switchData.block->statements.size() == 0);

    BOOST_TEST(switchData.block->cases.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0).IsCommon());
    BOOST_TEST(switchData.block->cases.at(0)->expr.IsCommon());
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0)->expr->expr.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::NameData>>(switchData.block->cases.at(0)->expr->expr.at(0).GetElement())->name.IsChange());
    BOOST_TEST(switchData.block->cases.at(0).ToString() == "foo|bar");

    BOOST_TEST(switchData.block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_case_expr_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { switch(1) { case 0: } }", "void foo() { switch(1) { case foo: } }"}});

   BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.block.IsCommon());
    BOOST_TEST(switchData.block->statements.size() == 0);

    BOOST_TEST(switchData.block->cases.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0).IsCommon());
    BOOST_TEST(switchData.block->cases.at(0)->expr.IsChange());
    BOOST_TEST(switchData.block->cases.at(0)->expr.GetOriginal()->expr.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0)->expr.GetOriginal()->expr.at(0).IsDelete());
    BOOST_TEST(switchData.block->cases.at(0)->expr.GetModified()->expr.size() == 1);
    BOOST_TEST(switchData.block->cases.at(0)->expr.GetModified()->expr.at(0).IsInsert());
    BOOST_TEST(switchData.block->cases.at(0).ToString() == "0|foo");

    BOOST_TEST(switchData.block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
