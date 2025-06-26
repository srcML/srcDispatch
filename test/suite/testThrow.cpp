// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testThrow.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE throw_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_change_common_throw) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { throw std::string(); }", "void foo() { throw std::string(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ThrowData>>() == "std::string()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_throw) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { throw Exception(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::ThrowData& throwData = *std::any_cast<std::shared_ptr<srcDispatch::ThrowData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(throwData.expr);
    BOOST_TEST(throwData.expr.IsInsert());
    BOOST_TEST(throwData.expr->expr.size() == 1);
    BOOST_TEST(throwData.expr->expr.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::CallData>>(throwData.expr->expr.at(0).GetElement())->name.IsInsert());
    BOOST_TEST(throwData.expr->expr.at(0).ToString<std::shared_ptr<srcDispatch::CallData>>() == "|Exception()");
    BOOST_TEST(throwData.expr.ToString() == "|Exception()");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ThrowData>>() == "|Exception()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_throw) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { throw Exception(); }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::ThrowData& throwData = *std::any_cast<std::shared_ptr<srcDispatch::ThrowData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(throwData.expr);
    BOOST_TEST(throwData.expr.IsDelete());
    BOOST_TEST(throwData.expr->expr.size() == 1);
    BOOST_TEST(throwData.expr->expr.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::CallData>>(throwData.expr->expr.at(0).GetElement())->name.IsDelete());
    BOOST_TEST(throwData.expr->expr.at(0).ToString<std::shared_ptr<srcDispatch::CallData>>() == "Exception()|");
    BOOST_TEST(throwData.expr.ToString() == "Exception()|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ThrowData>>() == "Exception()|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_throw) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { throw std::string(); }", "void foo() { throw Exception(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ThrowData& throwData = *std::any_cast<std::shared_ptr<srcDispatch::ThrowData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(throwData.expr);
    BOOST_TEST(throwData.expr.IsChange());
    BOOST_TEST(throwData.expr.GetOriginal()->expr.size() == 1);
    BOOST_TEST(throwData.expr.GetOriginal()->expr.at(0).IsDelete());
    BOOST_TEST(throwData.expr.GetOriginal()->expr.at(0).ToString<std::shared_ptr<srcDispatch::CallData>>() == "std::string()|");
    BOOST_TEST(throwData.expr.GetModified()->expr.size() == 1);
    BOOST_TEST(throwData.expr.GetModified()->expr.at(0).IsInsert());
    BOOST_TEST(throwData.expr.GetModified()->expr.at(0).ToString<std::shared_ptr<srcDispatch::CallData>>() == "|Exception()");
    BOOST_TEST(throwData.expr.ToString() == "std::string()|Exception()");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ThrowData>>() == "std::string()|Exception()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}