// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testDo.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE do_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <DoPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_change_common_do) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { do {} while(1); }", "void foo() { do {} while(1); }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::DoData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_do) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { do {} while(1);"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::DoData& doData = *std::any_cast<std::shared_ptr<srcDispatch::DoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(doData.condition);
    BOOST_TEST(doData.condition.IsInsert());
    BOOST_TEST(doData.condition->conditions.size() == 1);
    BOOST_TEST(doData.condition->conditions.at(0).IsInsert());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(doData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "|1");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "|1");
    BOOST_TEST(doData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()                  == "|1");
    BOOST_TEST(doData.condition.ToString()                                                                                      == "|1");

    BOOST_TEST(doData.block.IsInsert());
    BOOST_TEST(doData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::DoData>>() == "|1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_do) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { do {} while(1);", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::DoData& doData = *std::any_cast<std::shared_ptr<srcDispatch::DoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(doData.condition);
    BOOST_TEST(doData.condition.IsDelete());
    BOOST_TEST(doData.condition->conditions.size() == 1);
    BOOST_TEST(doData.condition->conditions.at(0).IsDelete());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(doData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "1|");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "1|");
    BOOST_TEST(doData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()                  == "1|");
    BOOST_TEST(doData.condition.ToString()                                                                                      == "1|");

    BOOST_TEST(doData.block.IsDelete());
    BOOST_TEST(doData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::DoData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_do) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { do { a; } while(1); }", "void foo() { do { b; } while(2); }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::DoData& deletedData = *std::any_cast<std::shared_ptr<srcDispatch::DoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(deletedData.condition.IsDelete());
    BOOST_TEST(deletedData.block.IsDelete());
    BOOST_TEST(deletedData.block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::DoData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).IsInsert());

    const srcDispatch::DoData& insertedData = *std::any_cast<std::shared_ptr<srcDispatch::DoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(1).GetElement());
    BOOST_TEST(insertedData.condition.IsInsert());
    BOOST_TEST(insertedData.block.IsInsert());
    BOOST_TEST(insertedData.block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).ToString<std::shared_ptr<srcDispatch::DoData>>() == "|2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
