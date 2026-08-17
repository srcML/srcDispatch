// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testFor.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE for_stmt tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <ForPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

// // for and control
BOOST_AUTO_TEST_CASE(block_common_for_cxx) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(; 1; ) {} }", "void foo() { for(; 1; ) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 0);
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "1");
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "; 1; ");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_insert_for_cxx) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() {}", "void foo() { for(; 1; ) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsInsert());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsInsert());
    BOOST_TEST(forData.control->init->inits.size() == 0);
    BOOST_TEST(forData.control->condition.IsInsert());
    BOOST_TEST(forData.control->condition.ToString() == "|1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsInsert());
    BOOST_TEST(forData.control->incr->exprs.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "|; 1; ");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_delete_for_cxx) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(; 1; ) {} }", "void foo() {}"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsDelete());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsDelete());
    BOOST_TEST(forData.control->init->inits.size() == 0);
    BOOST_TEST(forData.control->condition.IsDelete());
    BOOST_TEST(forData.control->condition.ToString() == "1|");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsDelete());
    BOOST_TEST(forData.control->incr->exprs.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "; 1; |");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(control_replace_for_cxx) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for(int a = 0; a < 10; ++a) {} }",
        "void foo() { for(int i = 0; i < 10; ++i) {} }"
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

    BOOST_TEST(forData.control->condition);
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition.ToString() == "a < 10|i < 10");

    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr.ToString() == "++ a|++ i");
    
    std::string controlDiffStr = runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>();
    BOOST_TEST(controlDiffStr == "int a = 0; a < 10; ++ a|int i = 0; i < 10; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(init_replace_for_cxx) {

    srcDispatch::DispatchRunner runner;
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

BOOST_AUTO_TEST_CASE(condition_replace_for_cxx) {

    srcDispatch::DispatchRunner runner;
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

BOOST_AUTO_TEST_CASE(incr_replace_for_cxx) {

    srcDispatch::DispatchRunner runner;
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