// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testGoto.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE goto_stmts tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <GotoPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// goto/break/continue
BOOST_AUTO_TEST_CASE(block_change_common_break) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { break; }", "void foo() { break; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsCommon());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::BREAK);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_insert_break) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { break; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsInsert());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::BREAK);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_delete_break) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { break; }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsDelete());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::BREAK);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_common_continue) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { continue; }", "void foo() { continue; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsCommon());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::CONTINUE);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_insert_continue) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { continue; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsInsert());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::CONTINUE);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_delete_continue) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { continue; }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsDelete());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::CONTINUE);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_common_goto) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { goto; }", "void foo() { goto; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsCommon());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::GOTO);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_insert_goto) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { goto; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsInsert());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::GOTO);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_delete_goto) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { goto; }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsDelete());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::GOTO);
    BOOST_TEST(!gotoData.label);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_common_label_goto) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { goto foo; }", "void foo() { goto foo; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsCommon());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::GOTO);
    BOOST_TEST(gotoData.label.IsCommon());
    BOOST_TEST(gotoData.label.ToString() == "foo");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_rename_label_goto) {

    srcDiffDispatch::srcDiffDispatchRunner runner;
    runner.RunDispatcher("void foo() { goto foo; }", "void foo() { goto bar; }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDiffDispatch::GotoData& gotoData = *std::any_cast<std::shared_ptr<srcDiffDispatch::GotoData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(gotoData.type.IsCommon());
    BOOST_TEST(gotoData.type.GetElement() == srcDiffDispatch::GotoData::GOTO);
    BOOST_TEST(gotoData.label.IsCommon());
    BOOST_TEST(gotoData.label->name.IsChange());
    BOOST_TEST(gotoData.label.ToString() == "foo|bar");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
