// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testLockStmt.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE lock_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <LockStmtPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(block_common_lock_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { lock (thisLock) {} }",
        "void foo() { lock (thisLock) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::LockStmtData& lockStmtData = *std::any_cast<std::shared_ptr<srcDispatch::LockStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(lockStmtData.init.IsCommon());
    BOOST_TEST(lockStmtData.init.ToString() == "thisLock");

    BOOST_TEST(lockStmtData.block.IsCommon());
    BOOST_TEST(lockStmtData.block->statements.size() == 0);
    BOOST_TEST(lockStmtData.block->labels.size()     == 0);
    BOOST_TEST(lockStmtData.block->cases.size()      == 0);
    BOOST_TEST(lockStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_lock_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() {}", "void foo() { lock (thisLock) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::LockStmtData& lockStmtData = *std::any_cast<std::shared_ptr<srcDispatch::LockStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    
    BOOST_TEST(lockStmtData.init.IsInsert());
    BOOST_TEST(lockStmtData.init.ToString() == "|thisLock");

    BOOST_TEST(lockStmtData.block.IsInsert());

    BOOST_TEST(lockStmtData.block->statements.size() == 0);
    BOOST_TEST(lockStmtData.block->labels.size()     == 0);
    BOOST_TEST(lockStmtData.block->cases.size()      == 0);
    BOOST_TEST(lockStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_lock_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() { lock (thisLock) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::LockStmtData& lockStmtData = *std::any_cast<std::shared_ptr<srcDispatch::LockStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(lockStmtData.init.IsDelete());
    BOOST_TEST(lockStmtData.init.ToString() == "thisLock|");

    BOOST_TEST(lockStmtData.block.IsDelete());

    BOOST_TEST(lockStmtData.block->statements.size() == 0);
    BOOST_TEST(lockStmtData.block->labels.size()     == 0);
    BOOST_TEST(lockStmtData.block->cases.size()      == 0);
    BOOST_TEST(lockStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(init_replace_lock_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { lock (apples) {} }",
        "void foo() { lock (oranges) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const srcDispatch::LockStmtData& lockStmtData = *std::any_cast<std::shared_ptr<srcDispatch::LockStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(lockStmtData.init.ToString() == "apples|oranges");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}