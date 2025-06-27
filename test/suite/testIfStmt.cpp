// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testIfStmt.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE if_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <IfStmtPolicy.hpp>
#include <IfPolicy.hpp>
#include <ElseIfPolicy.hpp>
#include <ElsePolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_change_common_if_stmt) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} }", "void foo() { if(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_if_stmt) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { if(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 1);
    BOOST_TEST(ifStmt.clauses.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "|1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "|1");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "|1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_if_stmt) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 1);
    BOOST_TEST(ifStmt.clauses.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1|");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_condition_if_stmt) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} }", "void foo() { if(2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 1);
    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1|2");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1|2");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1|2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_common_elseif) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} else if(2) {} }", "void foo() { if(1) {} else if(2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1");

    BOOST_TEST(ifStmt.clauses.at(1).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.ToString() == "2");
    BOOST_TEST(ifStmt.clauses.at(1).ToString<std::shared_ptr<srcDispatch::ElseIfData>>() == "2");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_elseif_whole) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { if(1) {} else if(2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "|1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "|1");

    BOOST_TEST(ifStmt.clauses.at(1).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.ToString() == "|2");
    BOOST_TEST(ifStmt.clauses.at(1).ToString<std::shared_ptr<srcDispatch::ElseIfData>>() == "|2");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "|1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_elseif_whole) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {if(1) {} else if(2) {} }", "void foo() { }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1|");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1|");

    BOOST_TEST(ifStmt.clauses.at(1).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.ToString() == "2|");
    BOOST_TEST(ifStmt.clauses.at(1).ToString<std::shared_ptr<srcDispatch::ElseIfData>>() == "2|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_elseif) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} }", "void foo() { if(1) {} else if(2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1");

    BOOST_TEST(ifStmt.clauses.at(1).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.ToString() == "|2");
    BOOST_TEST(ifStmt.clauses.at(1).ToString<std::shared_ptr<srcDispatch::ElseIfData>>() == "|2");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_elseif) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} else if(2) {} }", "void foo() { if(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1");

    BOOST_TEST(ifStmt.clauses.at(1).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::ElseIfData>>(ifStmt.clauses.at(1).GetElement())->condition.ToString() == "2|");
    BOOST_TEST(ifStmt.clauses.at(1).ToString<std::shared_ptr<srcDispatch::ElseIfData>>() == "2|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_common_else) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} else {} }", "void foo() { if(1) {} else {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1");

    BOOST_TEST(ifStmt.clauses.at(1).IsCommon());
    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::ElseData>>(ifStmt.clauses.at(1).GetElement())->condition);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_else_whole) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { if(1) {} else {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsInsert());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "|1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "|1");

    BOOST_TEST(ifStmt.clauses.at(1).IsInsert());
    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::ElseData>>(ifStmt.clauses.at(1).GetElement())->condition);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "|1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_else_whole) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {if(1) {} else {} }", "void foo() { }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsDelete());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1|");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1|");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1|");

    BOOST_TEST(ifStmt.clauses.at(1).IsDelete());
    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::ElseData>>(ifStmt.clauses.at(1).GetElement())->condition);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_else) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} }", "void foo() { if(1) {} else {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(ifStmt.clauses.at(1).IsInsert());
    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::ElseData>>(ifStmt.clauses.at(1).GetElement())->condition);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_else) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { if(1) {} else {} }", "void foo() { if(1) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::IfStmtData& ifStmt = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(ifStmt.clauses.size() == 2);

    BOOST_TEST(ifStmt.clauses.at(0).IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.IsCommon());
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifStmt.clauses.at(0).GetElement())->condition.ToString() == "1");
    BOOST_TEST(ifStmt.clauses.at(0).ToString<std::shared_ptr<srcDispatch::IfData>>() == "1");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(ifStmt.clauses.at(1).IsDelete());
    BOOST_TEST(!std::any_cast<std::shared_ptr<srcDispatch::ElseData>>(ifStmt.clauses.at(1).GetElement())->condition);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::IfStmtData>>() == "1");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
