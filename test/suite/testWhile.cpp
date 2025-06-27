// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testWhile.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE while_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <WhilePolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_common_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { while(1) {} }", "void foo() { while(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(whileData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "1");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "1");
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()               == "1");
    BOOST_TEST(whileData.condition.ToString()                                                                                   == "1");

    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size()   == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { while(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsInsert());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsInsert());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(whileData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "|1");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "|1");
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()               == "|1");
    BOOST_TEST(whileData.condition.ToString()                                                                                   == "|1");

    BOOST_TEST(whileData.block.IsInsert());
    BOOST_TEST(whileData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::WhileData>>() == "|1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { while(1) {}", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsDelete());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsDelete());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(whileData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "1|");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "1|");
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()               == "1|");
    BOOST_TEST(whileData.condition.ToString()                                                                                   == "1|");

    BOOST_TEST(whileData.block.IsDelete());
    BOOST_TEST(whileData.block->statements.size()   == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::WhileData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_condition_expr_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { while(1) {}", "void foo() { while(2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsChange());

    const srcDispatch::ExpressionData& originalExpr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(whileData.condition->conditions.at(0).GetOriginal());
    BOOST_TEST(originalExpr.expr.size() == 1);
    BOOST_TEST(originalExpr.expr.at(0).IsDelete());
    BOOST_TEST(originalExpr.expr.at(0).IsDelete());
    BOOST_TEST(originalExpr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>() == "1|");

    const srcDispatch::ExpressionData& modifiedExpr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(whileData.condition->conditions.at(0).GetModified());
    BOOST_TEST(modifiedExpr.expr.size() == 1);
    BOOST_TEST(modifiedExpr.expr.at(0).IsInsert());
    BOOST_TEST(modifiedExpr.expr.at(0).IsInsert());
    BOOST_TEST(modifiedExpr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>() == "|2");

    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()               == "1|2");
    BOOST_TEST(whileData.condition.ToString()                                                                                   == "1|2");

    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size()   == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::WhileData>>() == "1|2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { while(1) { a; }", "void foo() { while(2) { b; } }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::WhileData& deletedData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(deletedData.condition.IsDelete());
    BOOST_TEST(deletedData.block.IsDelete());
    BOOST_TEST(deletedData.block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::WhileData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).IsInsert());

    const srcDispatch::WhileData& insertedData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(1).GetElement());
    BOOST_TEST(insertedData.condition.IsInsert());
    BOOST_TEST(insertedData.block.IsInsert());
    BOOST_TEST(insertedData.block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).ToString<std::shared_ptr<srcDispatch::WhileData>>() == "|2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

