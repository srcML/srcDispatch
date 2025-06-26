// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testReturn.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE return_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_change_common_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return 0; }", "void foo() { return 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ReturnData>>() == "0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { return a; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(returnData.expr);
    BOOST_TEST(returnData.expr.IsInsert());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(returnData.expr->expr.at(0).GetElement())->name.IsInsert());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "|a");
    BOOST_TEST(returnData.expr.ToString() == "|a");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ReturnData>>() == "|a");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return a; }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(returnData.expr);
    BOOST_TEST(returnData.expr.IsDelete());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(returnData.expr->expr.at(0).GetElement())->name.IsDelete());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "a|");
    BOOST_TEST(returnData.expr.ToString() == "a|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ReturnData>>() == "a|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return a - b; }", "void foo() { return  a + b; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(returnData.expr);
    BOOST_TEST(returnData.expr.IsCommon());
    BOOST_TEST(returnData.expr->expr.size() == 3);
    BOOST_TEST(returnData.expr->expr.at(0).IsCommon());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "a");
    BOOST_TEST(returnData.expr->expr.at(1).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::OperatorData>>(returnData.expr->expr.at(1).GetElement())->op.IsChange());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::OperatorData>>(returnData.expr->expr.at(1).GetElement())->op.ToString() == "-|+");
    BOOST_TEST(returnData.expr->expr.at(1).ToString<std::shared_ptr<srcDiffDispatch::OperatorData>>() == "-|+");
    BOOST_TEST(returnData.expr->expr.at(2).IsCommon());
    BOOST_TEST(returnData.expr->expr.at(2).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "b");
    BOOST_TEST(returnData.expr.ToString() == "a - b|a + b");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ReturnData>>() == "a - b|a + b");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_return_expr) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return a; }", "void foo() { return b; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(returnData.expr);
    BOOST_TEST(returnData.expr.IsCommon());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(returnData.expr->expr.at(0).GetElement())->name.IsChange());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "a|b");
    BOOST_TEST(returnData.expr.ToString() == "a|b");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ReturnData>>() == "a|b");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_whole_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return 0; }", "void foo() { return a; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(returnData.expr);
    BOOST_TEST(returnData.expr.IsChange());

    BOOST_TEST(returnData.expr.GetOriginal()->expr.size() == 1);
    BOOST_TEST(returnData.expr.GetOriginal()->expr.at(0).IsDelete());
    BOOST_TEST(returnData.expr.GetOriginal()->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::LiteralData>>(returnData.expr.GetOriginal()->expr.at(0).GetElement())->literal.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::LiteralData>>(returnData.expr.GetOriginal()->expr.at(0).GetElement())->literal.ToString() == "0|");
    BOOST_TEST(returnData.expr.GetOriginal()->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0|");

    BOOST_TEST(returnData.expr.GetModified()->expr.size() == 1);
    BOOST_TEST(returnData.expr.GetModified()->expr.at(0).IsInsert());
    BOOST_TEST(returnData.expr.GetModified()->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(returnData.expr.GetModified()->expr.at(0).GetElement())->name.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(returnData.expr.GetModified()->expr.at(0).GetElement())->name.ToString() == "|a");
    BOOST_TEST(returnData.expr.GetModified()->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "|a");

    BOOST_TEST(returnData.expr.ToString() == "0|a");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ReturnData>>() == "0|a");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
