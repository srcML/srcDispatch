// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testSync.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE synchronized tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <SyncStmtPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(block_common_sync_stmt) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{
        "void foo() { synchronized (syncObj) {} }",
        "void foo() { synchronized (syncObj) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::SyncStmtData& syncStmtData = *std::any_cast<std::shared_ptr<srcDispatch::SyncStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(syncStmtData.init.IsCommon());
    BOOST_TEST(syncStmtData.init.ToString() == "syncObj");

    BOOST_TEST(syncStmtData.block.IsCommon());
    BOOST_TEST(syncStmtData.block->statements.size() == 0);
    BOOST_TEST(syncStmtData.block->labels.size()     == 0);
    BOOST_TEST(syncStmtData.block->cases.size()      == 0);
    BOOST_TEST(syncStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_sync_stmt) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"void foo() {}", "void foo() { synchronized (syncObj) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::SyncStmtData& syncStmtData = *std::any_cast<std::shared_ptr<srcDispatch::SyncStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    
    BOOST_TEST(syncStmtData.init.IsInsert());
    BOOST_TEST(syncStmtData.init.ToString() == "|syncObj");

    BOOST_TEST(syncStmtData.block.IsInsert());

    BOOST_TEST(syncStmtData.block->statements.size() == 0);
    BOOST_TEST(syncStmtData.block->labels.size()     == 0);
    BOOST_TEST(syncStmtData.block->cases.size()      == 0);
    BOOST_TEST(syncStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_sync_stmt) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"void foo() { synchronized (syncObj) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::SyncStmtData& syncStmtData = *std::any_cast<std::shared_ptr<srcDispatch::SyncStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(syncStmtData.init.IsDelete());
    BOOST_TEST(syncStmtData.init.ToString() == "syncObj|");

    BOOST_TEST(syncStmtData.block.IsDelete());

    BOOST_TEST(syncStmtData.block->statements.size() == 0);
    BOOST_TEST(syncStmtData.block->labels.size()     == 0);
    BOOST_TEST(syncStmtData.block->cases.size()      == 0);
    BOOST_TEST(syncStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
