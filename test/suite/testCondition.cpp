// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testCondition.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE condition tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <WhilePolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// condition decl
BOOST_AUTO_TEST_CASE(decl_condition_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { while(int i = 1) {} }", "void foo() { while(int i = 1) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDiffDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());

    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDiffDispatch::DeclData>>() == "int i = 1");
    BOOST_TEST(whileData.condition.ToString()                                                               == "int i = 1");

    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size()   == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::WhileData>>() == "int i = 1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(decl_condition_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { while(int i = 1) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDiffDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsInsert());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsInsert());
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDiffDispatch::DeclData>>() == "|int i = 1");
    BOOST_TEST(whileData.condition.ToString()                                                               == "|int i = 1");

    BOOST_TEST(whileData.block.IsInsert());
    BOOST_TEST(whileData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::WhileData>>() == "|int i = 1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(decl_condition_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { while(int i = 1) {} }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());


    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDiffDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsDelete());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsDelete());
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDiffDispatch::DeclData>>() == "int i = 1|");
    BOOST_TEST(whileData.condition.ToString()                                                               == "int i = 1|");

    BOOST_TEST(whileData.block.IsDelete());
    BOOST_TEST(whileData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::WhileData>>() == "int i = 1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(decl_condition_replace) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { while(int i = 1) {} }", "void foo() { while(double d = 1.0) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());


    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDiffDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 2);
    BOOST_TEST(whileData.condition->conditions.at(0).IsDelete());
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDiffDispatch::DeclData>>() == "int i = 1|");
    BOOST_TEST(whileData.condition->conditions.at(1).IsInsert());
    BOOST_TEST(whileData.condition->conditions.at(1).ToString<std::shared_ptr<srcDiffDispatch::DeclData>>() == "|double d = 1.0");
    BOOST_TEST(whileData.condition.ToString()                                                               == "int i = 1|double d = 1.0");

    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::WhileData>>() == "int i = 1|double d = 1.0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(decl_condition_modify) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { while(int i = 1) {} }", "void foo() { while(int i = 2) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());


    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDiffDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());
    BOOST_TEST(whileData.condition->conditions.at(0).ToString<std::shared_ptr<srcDiffDispatch::DeclData>>() == "int i = 1|int i = 2");
    BOOST_TEST(whileData.condition.ToString()                                                               == "int i = 1|int i = 2");

    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size()   == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::WhileData>>() == "int i = 1|int i = 2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
