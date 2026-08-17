// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testControl.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE control tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <ForPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(block_common_control) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; 1; ++i) {} }", "void foo() { for(int i = 0; 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_change_control) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i) {} }", "void foo() { for(int i = 1; i < 2; ++j) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0|int i = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1|i < 2");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i|++ j");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i|++ j");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i|int i = 1; i < 2; ++ j");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_expr_init_control_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0; i < 1; ++i) {} }", "void foo() { for(i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_expr_init_control_minor_change) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0; i < 1; ++i) {} }", "void foo() { for(i = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0|i = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0; i < 1; ++ i|i = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_expr_init_control_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0; i < 1; ++i) {} }", "void foo() { for(j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsChange());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0|j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0; i < 1; ++ i|j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_expr_init_control_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(; i < 1; ++i) {} }", "void foo() { for(i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "|i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "; i < 1; ++ i|i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_expr_init_control_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0; i < 1; ++i) {} }", "void foo() { for(; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0|");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0; i < 1; ++ i|; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_decl_init_control_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(; i < 1; ++i) {} }", "void foo() { for(int i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "|int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "; i < 1; ++ i|int i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_decl_init_control_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i) {} }", "void foo() { for(; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0|");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i|; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_range_insert_condition_incr_delete) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n = 0; n < 10; ++n) {} }",
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

    BOOST_TEST(forData->control->init.ToString() == "int n = 0|int n : numbers");

    const auto& declData = std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        forData->control->init->inits.at(0).GetElement()
    );
    
    BOOST_TEST(forData->control->condition);
    BOOST_TEST(forData->control->condition.IsDelete());
    BOOST_TEST(forData->control->condition.ToString() == "n < 10|");
    
    BOOST_TEST(forData->control->incr);
    BOOST_TEST(forData->control->incr.IsDelete());
    BOOST_TEST(forData->control->incr.ToString() == "++ n|");

    BOOST_TEST(declData->range);
    BOOST_TEST(declData->range.IsInsert());
    BOOST_TEST(declData->range.ToString() == "|numbers");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_range_replace) {
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

BOOST_AUTO_TEST_CASE(block_range_delete_condition_incr_insert) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{
        "void foo() { for (int n : numbers) {} }",
        "void foo() { for (int n = 0; n < 10; ++n) {} }"
    }});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);

    const auto& forData = std::any_cast<std::shared_ptr<srcDispatch::ForData>>(
        runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement()
    );

    BOOST_TEST(forData->control->init.ToString() == "int n : numbers|int n = 0");

    const auto& declData = std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        forData->control->init->inits.at(0).GetElement()
    );
    
    BOOST_TEST(forData->control->condition);
    BOOST_TEST(forData->control->condition.IsInsert());
    BOOST_TEST(forData->control->condition.ToString() == "|n < 10");
    
    BOOST_TEST(forData->control->incr);
    BOOST_TEST(forData->control->incr.IsInsert());
    BOOST_TEST(forData->control->incr.ToString() == "|++ n");

    BOOST_TEST(declData->range);
    BOOST_TEST(declData->range.IsDelete());
    BOOST_TEST(declData->range.ToString() == "numbers|");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_decl_to_expr) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i) {} }", "void foo() { for(i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0|");
    BOOST_TEST(forData.control->init->inits.at(1).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "|i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i|i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_expr_to_decl) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0; i < 1; ++i) {} }", "void foo() { for(int i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0|");
    BOOST_TEST(forData.control->init->inits.at(1).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "|int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0; i < 1; ++ i|int i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_decl) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0, j = 1; i < 1; ++i) {} }", "void foo() { for(int i = 0, j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0, int j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_decl_insert_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int j = 1; i < 1; ++i) {} }", "void foo() { for(int i = 0, j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "|int i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int j = 1; i < 1; ++ i|int i = 0, int j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_decl_insert_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i) {} }", "void foo() { for(int i = 0, j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "|int j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i|int i = 0, int j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_decl_insert_middle) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0, k = 2; i < 1; ++i) {} }", "void foo() { for(int i = 0, j = 1, k = 2; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 3);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "|int j = 1");
    BOOST_TEST(forData.control->init->inits.at(2).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(2).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int k = 2");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0, int k = 2; i < 1; ++ i|int i = 0, int j = 1, int k = 2; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_decl_delete_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0, j = 1; i < 1; ++i) {} }", "void foo() { for(int j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0|");
    BOOST_TEST(forData.control->init->inits.at(1).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0, int j = 1; i < 1; ++ i|int j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_decl_delete_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0, j = 1; i < 1; ++i) {} }", "void foo() { for(int i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int j = 1|");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0, int j = 1; i < 1; ++ i|int i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

// multi decl
BOOST_AUTO_TEST_CASE(block_init_control_multi_decl_delete_middle) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0, j = 1, k = 2; i < 1; ++i) {} }", "void foo() { for(int i = 0, k = 2; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 3);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int j = 1|");
    BOOST_TEST(forData.control->init->inits.at(2).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(2).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int k = 2");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0, int j = 1, int k = 2; i < 1; ++ i|int i = 0, int k = 2; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

// multi expr
BOOST_AUTO_TEST_CASE(block_init_control_multi_expr) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0, j = 1; i < 1; ++i) {} }", "void foo() { for(i = 0, j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0, j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_expr_insert_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(j = 1; i < 1; ++i) {} }", "void foo() { for(i = 0, j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "|i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "j = 1; i < 1; ++ i|i = 0, j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_expr_insert_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0; i < 1; ++i) {} }", "void foo() { for(i = 0, j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "|j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0; i < 1; ++ i|i = 0, j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_expr_insert_middle) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0, k = 2; i < 1; ++i) {} }", "void foo() { for(i = 0, j = 1, k = 2; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 3);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsInsert());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "|j = 1");
    BOOST_TEST(forData.control->init->inits.at(2).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(2).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "k = 2");    
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0, k = 2; i < 1; ++ i|i = 0, j = 1, k = 2; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_expr_delete_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0, j = 1; i < 1; ++i) {} }", "void foo() { for(j = 1; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0|");
    BOOST_TEST(forData.control->init->inits.at(1).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "j = 1");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0, j = 1; i < 1; ++ i|j = 1; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_expr_delete_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0, j = 1; i < 1; ++i) {} }", "void foo() { for(i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 2);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "j = 1|");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0, j = 1; i < 1; ++ i|i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_init_control_multi_expr_delete_middle) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(i = 0, j = 1, k = 2; i < 1; ++i) {} }", "void foo() { for(i = 0, k = 2; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 3);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "i = 0");
    BOOST_TEST(forData.control->init->inits.at(1).IsDelete());
    BOOST_TEST(forData.control->init->inits.at(1).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "j = 1|");
    BOOST_TEST(forData.control->init->inits.at(2).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(2).ToString<std::shared_ptr<srcDispatch::ExpressionData>>() == "k = 2");    
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 1);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "i = 0, j = 1, k = 2; i < 1; ++ i|i = 0, k = 2; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

// multi incr
BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i, --j) {} }", "void foo() { for(int i = 0; i < 1; ++i, --j) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 2);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "-- j");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i, -- j");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i, -- j");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr_insert_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; --j) {} }", "void foo() { for(int i = 0; i < 1; ++i, --j) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 2);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsInsert());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "|++ i");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "-- j");
    BOOST_TEST(forData.control->incr.ToString()              == "-- j|++ i, -- j");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; -- j|int i = 0; i < 1; ++ i, -- j");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr_insert_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i) {} }", "void foo() { for(int i = 0; i < 1; ++i, --j) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 2);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsInsert());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "|-- j");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i|++ i, -- j");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i|int i = 0; i < 1; ++ i, -- j");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr_insert_middle) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i, k /= 2) {} }", "void foo() { for(int i = 0; i < 1; ++i, --j, k /= 2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 3);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsInsert());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "|-- j");
    BOOST_TEST(forData.control->incr->exprs.at(2).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(2).ToString() == "k /= 2");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i, k /= 2|++ i, -- j, k /= 2");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i, k /= 2|int i = 0; i < 1; ++ i, -- j, k /= 2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr_delete_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i, --j) {} }", "void foo() { for(int i = 0; i < 1; --j) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 2);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsDelete());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i|");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "-- j");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i, -- j|-- j");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i, -- j|int i = 0; i < 1; -- j");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr_delete_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i, --j) {} }", "void foo() { for(int i = 0; i < 1; ++i) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 2);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsDelete());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "-- j|");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i, -- j|++ i");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i, -- j|int i = 0; i < 1; ++ i");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(block_incr_control_multi_expr_delete_middle) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"void foo() { for(int i = 0; i < 1; ++i, --j, k /= 2) {} }", "void foo() { for(int i = 0; i < 1; ++i, k /= 2) {} }"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    const srcDispatch::ForData& forData = *std::any_cast<std::shared_ptr<srcDispatch::ForData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());

    BOOST_TEST(forData.control.IsCommon());
    BOOST_TEST(forData.control->init);
    BOOST_TEST(forData.control->init.IsCommon());
    BOOST_TEST(forData.control->init->inits.size() == 1);
    BOOST_TEST(forData.control->init->inits.at(0).IsCommon());
    BOOST_TEST(forData.control->init->inits.at(0).ToString<std::shared_ptr<srcDispatch::DeclData>>() == "int i = 0");
    BOOST_TEST(forData.control->condition.IsCommon());
    BOOST_TEST(forData.control->condition->conditions.size() == 1);
    BOOST_TEST(forData.control->condition->conditions.at(0).IsCommon());
    BOOST_TEST(forData.control->condition.ToString() == "i < 1");
    BOOST_TEST(forData.control->incr);
    BOOST_TEST(forData.control->incr.IsCommon());
    BOOST_TEST(forData.control->incr->exprs.size() == 3);
    BOOST_TEST(forData.control->incr->exprs.at(0).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(0).ToString() == "++ i");
    BOOST_TEST(forData.control->incr->exprs.at(1).IsDelete());
    BOOST_TEST(forData.control->incr->exprs.at(1).ToString() == "-- j|");
    BOOST_TEST(forData.control->incr->exprs.at(2).IsCommon());
    BOOST_TEST(forData.control->incr->exprs.at(2).ToString() == "k /= 2");
    BOOST_TEST(forData.control->incr.ToString()              == "++ i, -- j, k /= 2|++ i, k /= 2");
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ToString<std::shared_ptr<srcDispatch::ForData>>() == "int i = 0; i < 1; ++ i, -- j, k /= 2|int i = 0; i < 1; ++ i, k /= 2");

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
