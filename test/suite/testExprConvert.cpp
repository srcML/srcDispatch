// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testExprConvert.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE expr convert tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <IfStmtPolicy.hpp>
#include <WhilePolicy.hpp>
#include <ForPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(decl_to_expr) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { int i = 0; }", "void foo() { 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDiffDispatch::DeclStmtData& declStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::DeclStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(declStmtData.decls.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0).IsDelete());
    BOOST_TEST(declStmtData.decls.at(0)->type.IsDelete());
    BOOST_TEST(declStmtData.decls.at(0)->type.ToString() == "int|");
    BOOST_TEST(declStmtData.decls.at(0)->name.IsDelete());
    BOOST_TEST(declStmtData.decls.at(0)->name.ToString() == "i|");
    BOOST_TEST(declStmtData.decls.at(0)->init.IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(declStmtData.decls.at(0)->init.ToString() == "0");
    BOOST_TEST(declStmtData.decls.at(0).ToString(srcDispatch::DELETE) == "int i = 0");

    const srcDiffDispatch::ExprStmtData& exprStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(exprStmtData.expr.IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.size() == 1);
    BOOST_TEST(exprStmtData.expr->expr.at(0).IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(exprStmtData.expr.ToString() == "0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(expr_to_decl) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { 0; }", "void foo() { int i = 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDiffDispatch::ExprStmtData& exprStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(exprStmtData.expr.IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.size() == 1);
    BOOST_TEST(exprStmtData.expr->expr.at(0).IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(exprStmtData.expr.ToString() == "0");

    const srcDiffDispatch::DeclStmtData& declStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::DeclStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(declStmtData.decls.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0).IsInsert());
    BOOST_TEST(declStmtData.decls.at(0)->type.IsInsert());
    BOOST_TEST(declStmtData.decls.at(0)->type.ToString() == "|int");
    BOOST_TEST(declStmtData.decls.at(0)->name.IsInsert());
    BOOST_TEST(declStmtData.decls.at(0)->name.ToString() == "|i");
    BOOST_TEST(declStmtData.decls.at(0)->init.IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(declStmtData.decls.at(0)->init.ToString() == "0");
    BOOST_TEST(declStmtData.decls.at(0).ToString(srcDispatch::INSERT) == "int i = 0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(expr_to_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { 0; }", "void foo() { return 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDiffDispatch::ExprStmtData& exprStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(exprStmtData.expr.IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.size() == 1);
    BOOST_TEST(exprStmtData.expr->expr.at(0).IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(exprStmtData.expr.ToString() == "0");

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(returnData.expr.IsCommon());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsCommon());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(returnData.expr.ToString() == "0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(return_to_expr) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return 0; }", "void foo() { 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(returnData.expr.IsCommon());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsCommon());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(returnData.expr.ToString() == "0");

    const srcDiffDispatch::ExprStmtData& exprStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ExprStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(exprStmtData.expr.IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.size() == 1);
    BOOST_TEST(exprStmtData.expr->expr.at(0).IsCommon());
    BOOST_TEST(exprStmtData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(exprStmtData.expr.ToString() == "0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(decl_to_return) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { int i = 0; }", "void foo() { return 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDiffDispatch::DeclStmtData& declStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::DeclStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(declStmtData.decls.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0).IsDelete());
    BOOST_TEST(declStmtData.decls.at(0)->type.IsDelete());
    BOOST_TEST(declStmtData.decls.at(0)->type.ToString() == "int|");
    BOOST_TEST(declStmtData.decls.at(0)->name.IsDelete());
    BOOST_TEST(declStmtData.decls.at(0)->name.ToString() == "i|");
    BOOST_TEST(declStmtData.decls.at(0)->init.IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(declStmtData.decls.at(0)->init.ToString() == "0");
    BOOST_TEST(declStmtData.decls.at(0).ToString(srcDispatch::DELETE) == "int i = 0");

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(returnData.expr.IsCommon());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsCommon());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(returnData.expr.ToString() == "0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(return_to_decl) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { return 0; }", "void foo() { int i = 0; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDiffDispatch::ReturnData& returnData = *std::any_cast<std::shared_ptr<srcDiffDispatch::ReturnData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(returnData.expr.IsCommon());
    BOOST_TEST(returnData.expr->expr.size() == 1);
    BOOST_TEST(returnData.expr->expr.at(0).IsCommon());
    BOOST_TEST(returnData.expr->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(returnData.expr.ToString() == "0");

    const srcDiffDispatch::DeclStmtData& declStmtData = *std::any_cast<std::shared_ptr<srcDiffDispatch::DeclStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(declStmtData.decls.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0).IsInsert());
    BOOST_TEST(declStmtData.decls.at(0)->type.IsInsert());
    BOOST_TEST(declStmtData.decls.at(0)->type.ToString() == "|int");
    BOOST_TEST(declStmtData.decls.at(0)->name.IsInsert());
    BOOST_TEST(declStmtData.decls.at(0)->name.ToString() == "|i");
    BOOST_TEST(declStmtData.decls.at(0)->init.IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.size() == 1);
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).IsCommon());
    BOOST_TEST(declStmtData.decls.at(0)->init->expr.at(0).ToString<std::shared_ptr<srcDiffDispatch::LiteralData>>() == "0");
    BOOST_TEST(declStmtData.decls.at(0)->init.ToString() == "0");
    BOOST_TEST(declStmtData.decls.at(0).ToString(srcDispatch::INSERT) == "int i = 0");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
