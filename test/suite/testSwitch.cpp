// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testSwitch.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE switch_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <SwitchPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// switch
BOOST_AUTO_TEST_CASE(block_change_common_switch) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { switch(1) {} }", "void foo() { switch(1) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::SwitchData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_switch) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { switch(1) {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());


    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.condition);
    BOOST_TEST(switchData.condition.IsInsert());
    BOOST_TEST(switchData.condition->conditions.size() == 1);
    BOOST_TEST(switchData.condition->conditions.at(0).IsInsert());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(switchData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "|1");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "|1");
    BOOST_TEST(switchData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()              == "|1");
    BOOST_TEST(switchData.condition.ToString()                                                                                  == "|1");

    BOOST_TEST(switchData.block.IsInsert());
    BOOST_TEST(switchData.block->statements.size()   == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::SwitchData>>() == "|1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_switch) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { switch(1) {}", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::SwitchData& switchData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(switchData.condition);
    BOOST_TEST(switchData.condition.IsDelete());
    BOOST_TEST(switchData.condition->conditions.size() == 1);
    BOOST_TEST(switchData.condition->conditions.at(0).IsDelete());

    const srcDispatch::ExpressionData& expr = *std::any_cast<std::shared_ptr<srcDispatch::ExpressionData>>(switchData.condition->conditions.at(0).GetElement());
    BOOST_TEST(expr.expr.size() == 1);
    BOOST_TEST(expr.expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.GetElement() == "1");
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::LiteralData>>(expr.expr.at(0).GetElement())->literal.ToString()   == "1|");
    BOOST_TEST(expr.expr.at(0).ToString<std::shared_ptr<srcDispatch::LiteralData>>()                                        == "1|");
    BOOST_TEST(switchData.condition->conditions.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>()              == "1|");
    BOOST_TEST(switchData.condition.ToString()                                                                                  == "1|");

    BOOST_TEST(switchData.block.IsDelete());
    BOOST_TEST(switchData.block->statements.size()   == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::SwitchData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_switch) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { switch(1) { default: a; } }", "void foo() { switch(2) { default: b; } }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 2);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::SwitchData& deletedData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(deletedData.condition.IsDelete());
    BOOST_TEST(deletedData.block.IsDelete());
    BOOST_TEST(deletedData.block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::SwitchData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).IsInsert());

    const srcDispatch::SwitchData& insertedData = *std::any_cast<std::shared_ptr<srcDispatch::SwitchData>>(runner.GetFunctionInfo().at(0)->block->statements.at(1).GetElement());
    BOOST_TEST(insertedData.condition.IsInsert());
    BOOST_TEST(insertedData.block.IsInsert());
    BOOST_TEST(insertedData.block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(1).ToString<std::shared_ptr<srcDispatch::SwitchData>>() == "|2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
