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

#include <ForPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
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



BOOST_AUTO_TEST_CASE(control_replace_foreach_java) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n : nums) {} }",
        "void foo() { for (int num : catalanNums) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& forData = std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(forData->control->init.ToString() == "int n : nums|int num : catalanNums");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(range_replace_foreach_java) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n : nums) {} }",
        "void foo() { for (int n : numbers) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& forData = std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(forData->control->init.ToString() == "int n : nums|int n : numbers");

    const auto& declData = std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        forData->control->init->inits.at(0).GetElement()
    );
    BOOST_TEST(declData->range.ToString() == "nums|numbers");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(init_replace_foreach_java) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n : fibNums) {} }",
        "void foo() { for (int num : fibNums) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& forData = std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );
    
    BOOST_TEST(forData->control->init.ToString() == "int n : fibNums|int num : fibNums");

    const auto& declData = std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        forData->control->init->inits.at(0).GetElement()
    );
    BOOST_TEST(declData->name.ToString() == "n|num");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}