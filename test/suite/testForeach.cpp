// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testForeach.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE for_each tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <ForeachPolicy.hpp>
#include <ForPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(block_common_foreach) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { foreach (string fruit in fruits) {} }",
        "void foo() { foreach (string fruit in fruits) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForeachData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsCommon());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsCommon());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");
    
    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForeachData>>() == "string fruit : fruits");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_foreach) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() {}", "void foo() { foreach (string fruit in fruits) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::ForeachData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsInsert());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsInsert());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");

    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForeachData>>() == "|string fruit : fruits");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_foreach) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{"void foo() { foreach (string fruit in fruits) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::ForeachData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsDelete());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsDelete());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");

    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForeachData>>() == "string fruit : fruits|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_foreach) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { foreach (string fruit in fruits) {} }",
        "void foo() { foreach (var obj in objects) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& foreachData = std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    foreachData->control->init.IsChange();

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

/**
 * @section Tests ranged-based for (C++ Foreach)
 */
BOOST_AUTO_TEST_CASE(block_common_foreach_cxx) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n : nums) {} }",
        "void foo() { for (int n : nums) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsCommon());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsCommon());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");
    
    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForData>>() == "int n : nums");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_foreach_cxx) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { for (int n : nums) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::ForData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsInsert());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsInsert());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");

    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForData>>() == "|int n : nums");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_foreach_cxx) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for (int n : nums) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::ForData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsDelete());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsDelete());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");

    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForData>>() == "int n : nums|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_foreach_cxx) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n : nums) {} }",
        "void foo() { for (int nums : numbers) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& forData = std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    forData->control->init.IsChange();

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

/**
 * @section Tests Java foreach
 */
BOOST_AUTO_TEST_CASE(block_common_foreach_java) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{
        "void foo() { for (int n : nums) {} }",
        "void foo() { for (int n : nums) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsCommon());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsCommon());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");
    
    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForData>>() == "int n : nums");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_foreach_java) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"void foo() {}", "void foo() { for (int n : nums) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::ForData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsInsert());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsInsert());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");

    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForData>>() == "|int n : nums");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_foreach_java) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{"void foo() { for (int n : nums) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::ForData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData.control.IsDelete());

    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.control->init.IsDelete());
    BOOST_TEST(foreachData.control->init->inits.size() == 1);
    BOOST_TEST(foreachData.control->condition.ToString() == "");

    const auto& deltaForeach = runner.GetFunctionInfo().at(0)->block->statements.at(0);
    BOOST_TEST(deltaForeach.ToString<std::shared_ptr<srcDispatch::ForData>>() == "int n : nums|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_replace_foreach_java) {
    srcDispatch::DispatchRunner runner("Java");
    runner.RunDispatcher({{
        "void foo() { for (int n : nums) {} }",
        "void foo() { for (int nums : numbers) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& forData = std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    forData->control->init.IsChange();

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}