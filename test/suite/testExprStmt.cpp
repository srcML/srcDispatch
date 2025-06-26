// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testExprStmt.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE expr_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_change_common_expr_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { a; }", "void foo() { a; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "a");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_expr_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { a; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

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

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_expr_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { a; }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::ExprStmtData& expr = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(expr.expr);
    BOOST_TEST(expr.expr.IsDelete());
    BOOST_TEST(expr.expr->expr.size() == 1);
    BOOST_TEST(expr.expr->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(expr.expr->expr.at(0).GetElement())->name.IsDelete());
    BOOST_TEST(expr.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "a|");
    BOOST_TEST(expr.expr.ToString() == "a|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "a|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_expr_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { a - b; }", "void foo() { a + b; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& expr = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(expr.expr);
    BOOST_TEST(expr.expr.IsCommon());
    BOOST_TEST(expr.expr->expr.size() == 3);
    BOOST_TEST(expr.expr->expr.at(0).IsCommon());
    BOOST_TEST(expr.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "a");
    BOOST_TEST(expr.expr->expr.at(1).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::OperatorData>>(expr.expr->expr.at(1).GetElement())->op.IsChange());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::OperatorData>>(expr.expr->expr.at(1).GetElement())->op.ToString() == "-|+");
    BOOST_TEST(expr.expr->expr.at(1).ToString<std::shared_ptr<srcDiffDispatch::OperatorData>>() == "-|+");
    BOOST_TEST(expr.expr->expr.at(2).IsCommon());
    BOOST_TEST(expr.expr->expr.at(2).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "b");
    BOOST_TEST(expr.expr.ToString() == "a - b|a + b");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "a - b|a + b");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_expr_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { a; }", "void foo() { b; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::ExprStmtData& deletedExpr = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(deletedExpr.expr);
    BOOST_TEST(deletedExpr.expr.IsDelete());
    BOOST_TEST(deletedExpr.expr->expr.size() == 1);
    BOOST_TEST(deletedExpr.expr->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(deletedExpr.expr->expr.at(0).GetElement())->name.IsDelete());
    BOOST_TEST(deletedExpr.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "a|");
    BOOST_TEST(deletedExpr.expr.ToString() == "a|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "a|");


    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).IsInsert());

    const srcDiffDispatch::ExprStmtData& insertedExpr = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(1).GetElement());
    BOOST_TEST(insertedExpr.expr);
    BOOST_TEST(insertedExpr.expr.IsInsert());
    BOOST_TEST(insertedExpr.expr->expr.size() == 1);
    BOOST_TEST(insertedExpr.expr->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDiffDispatch::NameData>>(insertedExpr.expr->expr.at(0).GetElement())->name.IsInsert());
    BOOST_TEST(insertedExpr.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::NameData>>() == "|b");
    BOOST_TEST(insertedExpr.expr.ToString() == "|b");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "|b");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
