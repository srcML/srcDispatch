// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testUsingStmt.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE using_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <UsingStmtPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(block_common_using_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { using (StreamReader sr = new StreamReader(\"TestFile.txt\")) {} }",
        "void foo() { using (StreamReader sr = new StreamReader(\"TestFile.txt\")) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::UsingStmtData& usingStmtData = *std::any_cast<std::shared_ptr<srcDispatch::UsingStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(usingStmtData.init.IsCommon());
    BOOST_TEST(usingStmtData.init.ToString() == "StreamReader sr = new StreamReader(\"TestFile.txt\")");

    BOOST_TEST(usingStmtData.block.IsCommon());
    BOOST_TEST(usingStmtData.block->statements.size() == 0);
    BOOST_TEST(usingStmtData.block->labels.size()     == 0);
    BOOST_TEST(usingStmtData.block->cases.size()      == 0);
    BOOST_TEST(usingStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_using_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() {}", "void foo() { using (StreamReader sr = new StreamReader(\"TestFile.txt\")) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::UsingStmtData& usingStmtData = *std::any_cast<std::shared_ptr<srcDispatch::UsingStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    
    BOOST_TEST(usingStmtData.init.IsInsert());
    BOOST_TEST(usingStmtData.init.ToString() == "|StreamReader sr = new StreamReader(\"TestFile.txt\")");

    BOOST_TEST(usingStmtData.block.IsInsert());

    BOOST_TEST(usingStmtData.block->statements.size() == 0);
    BOOST_TEST(usingStmtData.block->labels.size()     == 0);
    BOOST_TEST(usingStmtData.block->cases.size()      == 0);
    BOOST_TEST(usingStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_using_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() { using (StreamReader sr = new StreamReader(\"TestFile.txt\")) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::UsingStmtData& usingStmtData = *std::any_cast<std::shared_ptr<srcDispatch::UsingStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(usingStmtData.init.IsDelete());
    BOOST_TEST(usingStmtData.init.ToString() == "StreamReader sr = new StreamReader(\"TestFile.txt\")|");

    BOOST_TEST(usingStmtData.block.IsDelete());

    BOOST_TEST(usingStmtData.block->statements.size() == 0);
    BOOST_TEST(usingStmtData.block->labels.size()     == 0);
    BOOST_TEST(usingStmtData.block->cases.size()      == 0);
    BOOST_TEST(usingStmtData.block->blocks.size()     == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(init_replace_using_stmt) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { using (StreamReader sr = new StreamReader(\"TestFile.txt\")) {} }",
        "void foo() { using (sr) {} }",
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const srcDispatch::UsingStmtData& usingStmtData = *std::any_cast<std::shared_ptr<srcDispatch::UsingStmtData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(usingStmtData.init.ToString() == "StreamReader sr = new StreamReader(\"TestFile.txt\")|sr");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}