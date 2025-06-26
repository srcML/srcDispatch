// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testCall.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE call tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(call_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(); }", "void foo() { f(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 0);
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f()");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_insert_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { f(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsInsert());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsInsert());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "|f");
    BOOST_TEST(call.arguments.size() == 0);
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "|f()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "|f()");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_delete_stmt) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(); }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsDelete());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsDelete());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f|");
    BOOST_TEST(call.arguments.size() == 0);
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f()|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f()|");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { a = b; }", "void foo() { a = b + f(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 5);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());
    BOOST_TEST(stmt.expr->expr.at(1).IsCommon());
    BOOST_TEST(stmt.expr->expr.at(2).IsCommon());
    BOOST_TEST(stmt.expr->expr.at(3).IsInsert());
    BOOST_TEST(stmt.expr->expr.at(4).IsInsert());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(4).GetElement());
    BOOST_TEST(call.name.ToString() == "|f");
    BOOST_TEST(call.arguments.size() == 0);
    BOOST_TEST(stmt.expr->expr.at(4).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "|f()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "a = b|a = b + f()");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { a = b + f(); }", "void foo() { a = b; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 5);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());
    BOOST_TEST(stmt.expr->expr.at(1).IsCommon());
    BOOST_TEST(stmt.expr->expr.at(2).IsCommon());
    BOOST_TEST(stmt.expr->expr.at(3).IsDelete());
    BOOST_TEST(stmt.expr->expr.at(4).IsDelete());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(4).GetElement());
    BOOST_TEST(call.name.ToString() == "f|");
    BOOST_TEST(call.arguments.size() == 0);
    BOOST_TEST(stmt.expr->expr.at(4).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f()|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "a = b + f()|a = b");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

// args
BOOST_AUTO_TEST_CASE(call_arg_common) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(a); }", "void foo() { f(a); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 1);
    BOOST_TEST(call.arguments.at(0).IsCommon());
    BOOST_TEST(call.arguments.at(0).ToString() == "a");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f(a)");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f(a)");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_arg_insert) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(); }", "void foo() { f(a); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 1);
    BOOST_TEST(call.arguments.at(0).IsInsert());
    BOOST_TEST(call.arguments.at(0).ToString() == "|a");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f()|f(a)");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f()|f(a)");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_arg_delete) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(a); }", "void foo() { f(); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 1);
    BOOST_TEST(call.arguments.at(0).IsDelete());
    BOOST_TEST(call.arguments.at(0).ToString() == "a|");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f(a)|f()");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f(a)|f()");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_arg_insert_front) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(a); }", "void foo() { f(0, a); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 2);
    BOOST_TEST(call.arguments.at(0).IsInsert());
    BOOST_TEST(call.arguments.at(0).ToString() == "|0");
    BOOST_TEST(call.arguments.at(1).IsCommon());
    BOOST_TEST(call.arguments.at(1).ToString() == "a");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f(a)|f(0, a)");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f(a)|f(0, a)");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_arg_insert_back) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(a); }", "void foo() { f(a, b + 1); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 2);
    BOOST_TEST(call.arguments.at(0).IsCommon());
    BOOST_TEST(call.arguments.at(0).ToString() == "a");
    BOOST_TEST(call.arguments.at(1).IsInsert());
    BOOST_TEST(call.arguments.at(1).ToString() == "|b + 1");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f(a)|f(a, b + 1)");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f(a)|f(a, b + 1)");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}



BOOST_AUTO_TEST_CASE(call_arg_delete_front) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(0, a); }", "void foo() { f(a); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 2);
    BOOST_TEST(call.arguments.at(0).IsDelete());
    BOOST_TEST(call.arguments.at(0).ToString() == "0|");
    BOOST_TEST(call.arguments.at(1).IsCommon());
    BOOST_TEST(call.arguments.at(1).ToString() == "a");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f(0, a)|f(a)");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f(0, a)|f(a)");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(call_arg_delete_back) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { f(a, b + 1); }", "void foo() { f(a); }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::ExprStmtData& stmt = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(stmt.expr.IsCommon());
    BOOST_TEST(stmt.expr->expr.size() == 1);
    BOOST_TEST(stmt.expr->expr.at(0).IsCommon());

    const srcDiffDispatch::CallData&     call = *std::any_cast<std::shared_ptr<srcDiffDispatch::CallData>>(stmt.expr->expr.at(0).GetElement());
    BOOST_TEST(call.name.ToString() == "f");
    BOOST_TEST(call.arguments.size() == 2);
    BOOST_TEST(call.arguments.at(0).IsCommon());
    BOOST_TEST(call.arguments.at(0).ToString() == "a");
    BOOST_TEST(call.arguments.at(1).IsDelete());
    BOOST_TEST(call.arguments.at(1).ToString() == "b + 1|");
    BOOST_TEST(stmt.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::CallData>>() == "f(a, b + 1)|f(a)");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDiffDispatch::ExprStmtData>>() == "f(a, b + 1)|f(a)");
    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
