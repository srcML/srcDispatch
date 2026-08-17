// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testForeachCS.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE for_each_cs tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <ForeachPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(block_common_foreach_cs) {
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

BOOST_AUTO_TEST_CASE(block_insert_foreach_cs) {
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

BOOST_AUTO_TEST_CASE(block_delete_foreach_cs) {
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

BOOST_AUTO_TEST_CASE(control_replace_foreach_cs) {
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

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(range_replace_foreach_cxx) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { foreach (int n in nums) {} }",
        "void foo() { foreach (int n in fibs) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& foreachData = std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(foreachData->control->init.ToString() == "int n : nums|int n : fibs");

    const auto& declData = std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        foreachData->control->init->inits.at(0).GetElement()
    );
    BOOST_TEST(declData->range.ToString() == "nums|fibs");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(init_replace_foreach_cxx) {
    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { foreach (int n in fibs) {} }",
        "void foo() { foreach (int num in fibs) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& foreachData = std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    
    BOOST_TEST(foreachData->control->init.ToString() == "int n : fibs|int num : fibs");

    const auto& declData = std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        foreachData->control->init->inits.at(0).GetElement()
    );
    BOOST_TEST(declData->name.ToString() == "n|num");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}