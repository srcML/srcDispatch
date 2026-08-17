// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testControl.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE control_cs tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <ForPolicy.hpp>
#include <ForeachPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(init_replace_for) {

    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { for(int a = 0; a < 10; ++a) {} }",
        "void foo() { for(int i = 0; a < 10; ++a) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init.ToString() == "int a = 0|int i = 0");
    
    std::string controlDiffStr = runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>();
    BOOST_TEST(controlDiffStr == "int a = 0; a < 10; ++ a|int i = 0; a < 10; ++ a");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(range_replace_foreach) {
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

BOOST_AUTO_TEST_CASE(condition_replace_for) {

    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { for(int a = 0; a < 10; ++a) {} }",
        "void foo() { for(int a = 0; i < 10; ++a) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control->condition);
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition.ToString() == "a < 10|i < 10");
    
    std::string controlDiffStr = runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>();
    BOOST_TEST(controlDiffStr == "int a = 0; a < 10; ++ a|int a = 0; i < 10; ++ a");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(incr_replace_for) {

    srcDispatch::DispatchRunner runner("C#");
    runner.RunDispatcher({{
        "void foo() { for(int a = 0; a < 10; ++a) {} }",
        "void foo() { for(int a = 0; a < 10; ++i) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr.ToString() == "++ a|++ i");
    
    std::string controlDiffStr = runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>();
    BOOST_TEST(controlDiffStr == "int a = 0; a < 10; ++ a|int a = 0; a < 10; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}