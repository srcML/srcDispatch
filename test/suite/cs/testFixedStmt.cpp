// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testFixedStmt.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE fixed_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <FixedStmtPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(block_common_fixed_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { fixed (int* p = &pt.x) {} }",
        "void foo() { fixed (int* p = &pt.x) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::FixedStmtData& fixedStmtData = *std::any_cast<std::shared_ptr<srcDispatch::FixedStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(fixedStmtData.init.IsCommon());
    BOOST_TEST(fixedStmtData.init.ToString() == "int * p = & pt.x");

    BOOST_TEST(fixedStmtData.block.IsCommon());
    BOOST_TEST(fixedStmtData.block->statements.size() == 0);
    BOOST_TEST(fixedStmtData.block->labels.size()     == 0);
    BOOST_TEST(fixedStmtData.block->cases.size()      == 0);
    BOOST_TEST(fixedStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_fixed_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() {}", "void foo() { fixed (int* p = &pt.x) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::FixedStmtData& fixedStmtData = *std::any_cast<std::shared_ptr<srcDispatch::FixedStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    
    BOOST_TEST(fixedStmtData.init.IsInsert());
    BOOST_TEST(fixedStmtData.init.ToString() == "|int * p = & pt.x");

    BOOST_TEST(fixedStmtData.block.IsInsert());

    BOOST_TEST(fixedStmtData.block->statements.size() == 0);
    BOOST_TEST(fixedStmtData.block->labels.size()     == 0);
    BOOST_TEST(fixedStmtData.block->cases.size()      == 0);
    BOOST_TEST(fixedStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_fixed_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() { fixed (int* p = &pt.x) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::FixedStmtData& fixedStmtData = *std::any_cast<std::shared_ptr<srcDispatch::FixedStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(fixedStmtData.init.IsDelete());
    BOOST_TEST(fixedStmtData.init.ToString() == "int * p = & pt.x|");

    BOOST_TEST(fixedStmtData.block.IsDelete());

    BOOST_TEST(fixedStmtData.block->statements.size() == 0);
    BOOST_TEST(fixedStmtData.block->labels.size()     == 0);
    BOOST_TEST(fixedStmtData.block->cases.size()      == 0);
    BOOST_TEST(fixedStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(init_replace_fixed_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { fixed (int* p = &pt.x) {} }",
        "void foo() { fixed (int* nums = numbers) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const srcDispatch::FixedStmtData& fixedStmtData = *std::any_cast<std::shared_ptr<srcDispatch::FixedStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(fixedStmtData.init.ToString() == "int * p = & pt.x|int * nums = numbers");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}