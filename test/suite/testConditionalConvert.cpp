// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testConditionalConvert.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE conditional convert tests
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

BOOST_AUTO_TEST_CASE(if_to_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { if(1) {} }", "void foo() { while(1) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::IfStmtData& ifData = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(ifData.clauses.size() == 1);

    const srcDispatch::IfData& ifClause = *std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifData.clauses.at(0).GetElement());
    BOOST_TEST(ifData.clauses.at(0).IsDelete());
    BOOST_TEST(ifClause.condition.IsCommon());
    BOOST_TEST(ifClause.condition->conditions.size() == 1);
    BOOST_TEST(ifClause.condition->conditions.at(0).IsCommon());
    BOOST_TEST(ifClause.condition.ToString() == "1");
    BOOST_TEST(ifClause.block.IsCommon());
    BOOST_TEST(ifClause.block->statements.size() == 0);

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());
    BOOST_TEST(whileData.condition.ToString() == "1");
    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(while_to_if) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { while(1) {} }", "void foo() { if(1) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());
    BOOST_TEST(whileData.condition.ToString() == "1");
    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size() == 0);

    const srcDispatch::IfStmtData& ifData = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(ifData.clauses.size() == 1);

    const srcDispatch::IfData& ifClause = *std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifData.clauses.at(0).GetElement());
    BOOST_TEST(ifData.clauses.at(0).IsInsert());
    BOOST_TEST(ifClause.condition.IsCommon());
    BOOST_TEST(ifClause.condition->conditions.size() == 1);
    BOOST_TEST(ifClause.condition->conditions.at(0).IsCommon());
    BOOST_TEST(ifClause.condition.ToString() == "1");
    BOOST_TEST(ifClause.block.IsCommon());
    BOOST_TEST(ifClause.block->statements.size() == 0);


    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(while_to_if_with_convert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { while(1) { while(2) {} } }", "void foo() { if(1) { if(2) {} } }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(whileData.condition);
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());
    BOOST_TEST(whileData.condition.ToString() == "1");

    const srcDispatch::IfStmtData& ifData = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(ifData.clauses.size() == 1);

    const srcDispatch::IfData& ifClause = *std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifData.clauses.at(0).GetElement());
    BOOST_TEST(ifData.clauses.at(0).IsInsert());
    BOOST_TEST(ifClause.condition.IsCommon());
    BOOST_TEST(ifClause.condition->conditions.size() == 1);
    BOOST_TEST(ifClause.condition->conditions.at(0).IsCommon());
    BOOST_TEST(ifClause.condition.ToString() == "1");
    BOOST_TEST(ifClause.block.IsCommon());
    BOOST_TEST(ifClause.block->statements.size() == 1);

    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(if_to_for) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { if(i < 10) { sum += i; } }", "void foo() { for(int i = 0; i < 10; ++i) { sum += i; } }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::IfStmtData& ifData = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(ifData.clauses.size() == 1);

    const srcDispatch::IfData& ifClause = *std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifData.clauses.at(0).GetElement());
    BOOST_TEST(ifData.clauses.at(0).IsDelete());
    BOOST_TEST(ifClause.condition.IsCommon());
    BOOST_TEST(ifClause.condition->conditions.size() == 1);
    BOOST_TEST(ifClause.condition->conditions.at(0).IsCommon());
    BOOST_TEST(ifClause.condition.ToString() == "i < 10");
    BOOST_TEST(ifClause.block.IsCommon());
    BOOST_TEST(ifClause.block->statements.size() == 1);

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(forData.control);
    BOOST_TEST(forData.control.IsInsert());

    BOOST_TEST(forData.control->init.IsInsert());
    BOOST_TEST(forData.control->init.ToString() == "|int i = 0");

    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 10");

    BOOST_TEST(forData.control->incr.IsInsert());
    BOOST_TEST(forData.control->incr.ToString() == "|++ i");

    BOOST_TEST(forData.control.ToString(srcDispatch::INSERT) == "int i = 0; i < 10; ++ i");

    BOOST_TEST(forData.block.IsCommon());
    BOOST_TEST(forData.block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(for_to_if) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { for(int i = 0; i < 10; ++i) { sum += i; } }", "void foo() { if(i < 10) { sum += i; } }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(forData.control);
    BOOST_TEST(forData.control.IsDelete());

    BOOST_TEST(forData.control->init.IsDelete());
    BOOST_TEST(forData.control->init.ToString() == "int i = 0|");

    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 10");

    BOOST_TEST(forData.control->incr.IsDelete());
    BOOST_TEST(forData.control->incr.ToString() == "++ i|");

    BOOST_TEST(forData.control.ToString(srcDispatch::DELETE) == "int i = 0; i < 10; ++ i");

    BOOST_TEST(forData.block.IsCommon());
    BOOST_TEST(forData.block->statements.size() == 1);

    const srcDispatch::IfStmtData& ifData = *std::any_cast<std::shared_ptr<srcDispatch::IfStmtData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(ifData.clauses.size() == 1);

    const srcDispatch::IfData& ifClause = *std::any_cast<std::shared_ptr<srcDispatch::IfData>>(ifData.clauses.at(0).GetElement());
    BOOST_TEST(ifData.clauses.at(0).IsInsert());
    BOOST_TEST(ifClause.condition.IsCommon());
    BOOST_TEST(ifClause.condition->conditions.size() == 1);
    BOOST_TEST(ifClause.condition->conditions.at(0).IsCommon());
    BOOST_TEST(ifClause.condition.ToString() == "i < 10");
    BOOST_TEST(ifClause.block.IsCommon());
    BOOST_TEST(ifClause.block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(while_to_for) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { while(i < 10) { sum += i; } }", "void foo() { for(int i = 0; i < 10; ++i) { sum += i; } }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());
    BOOST_TEST(whileData.condition.ToString() == "i < 10");
    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size() == 1);

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(forData.control);
    BOOST_TEST(forData.control.IsInsert());

    BOOST_TEST(forData.control->init.IsInsert());
    BOOST_TEST(forData.control->init.ToString() == "|int i = 0");

    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 10");

    BOOST_TEST(forData.control->incr.IsInsert());
    BOOST_TEST(forData.control->incr.ToString() == "|++ i");

    BOOST_TEST(forData.control.ToString(srcDispatch::INSERT) == "int i = 0; i < 10; ++ i");

    BOOST_TEST(forData.block.IsCommon());
    BOOST_TEST(forData.block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(for_to_while) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { for(int i = 0; i < 10; ++i) { sum += i; } }", "void foo() { while(i < 10) { sum += i; } }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsChange());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetOriginal());
    BOOST_TEST(forData.control);
    BOOST_TEST(forData.control.IsDelete());

    BOOST_TEST(forData.control->init.IsDelete());
    BOOST_TEST(forData.control->init.ToString() == "int i = 0|");

    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 10");

    BOOST_TEST(forData.control->incr.IsDelete());
    BOOST_TEST(forData.control->incr.ToString() == "++ i|");

    BOOST_TEST(forData.control.ToString(srcDispatch::DELETE) == "int i = 0; i < 10; ++ i");

    BOOST_TEST(forData.block.IsCommon());
    BOOST_TEST(forData.block->statements.size() == 1);

    const srcDispatch::WhileData& whileData = *std::any_cast<std::shared_ptr<srcDispatch::WhileData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetModified());
    BOOST_TEST(whileData.condition.IsCommon());
    BOOST_TEST(whileData.condition->conditions.size() == 1);
    BOOST_TEST(whileData.condition->conditions.at(0).IsCommon());
    BOOST_TEST(whileData.condition.ToString() == "i < 10");
    BOOST_TEST(whileData.block.IsCommon());
    BOOST_TEST(whileData.block->statements.size() == 1);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
