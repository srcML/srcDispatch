// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testIfStmt.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE try_catch tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

#include <TryPolicy.hpp>
#include <CatchPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(try_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} }", "void foo() { try {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());
    BOOST_TEST(bool(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->block));
    BOOST_TEST(bool(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->block.IsCommon()));
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->clauses.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(try_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() {}", "void foo() { try {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsInsert());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).ModifiedType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());
    BOOST_TEST(bool(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->block));
    BOOST_TEST(bool(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->block.IsInsert()));
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->clauses.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(try_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} }", "void foo() {}");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsDelete());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());
    BOOST_TEST(bool(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->block));
    BOOST_TEST(bool(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->block.IsDelete()));
    BOOST_TEST(std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement())->clauses.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch() {} }", "void foo() { try {} catch() {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 0);
    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_insert) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} }", "void foo() { try {} catch() {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsInsert());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 0);
    BOOST_TEST(catchData.block.IsInsert());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_delete) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch() {} }", "void foo() { try {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsDelete());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 0);
    BOOST_TEST(catchData.block.IsDelete());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common_param) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(Exception e) {} }", "void foo() { try {} catch(Exception e) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 1);
    BOOST_TEST(catchData.parameters.at(0).IsCommon());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "Exception e");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_insert_param) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch() {} }", "void foo() { try {} catch(Exception e) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 1);
    BOOST_TEST(catchData.parameters.at(0).IsInsert());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "|Exception e");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_delete_param) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(Exception e) {} }", "void foo() { try {} catch() {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 1);
    BOOST_TEST(catchData.parameters.at(0).IsDelete());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "Exception e|");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common_param_insert_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(Exception e) {} }", "void foo() { try {} catch(foo bar, Exception e) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 2);
    BOOST_TEST(catchData.parameters.at(0).IsInsert());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "|foo bar");
    BOOST_TEST(catchData.parameters.at(1).IsCommon());
    BOOST_TEST(catchData.parameters.at(1).ToString() == "Exception e");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common_param_insert_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(Exception e) {} }", "void foo() { try {} catch(Exception e, foo bar) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 2);
    BOOST_TEST(catchData.parameters.at(0).IsCommon());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "Exception e");
    BOOST_TEST(catchData.parameters.at(1).IsInsert());
    BOOST_TEST(catchData.parameters.at(1).ToString() == "|foo bar");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common_param_delete_front) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(foo bar, Exception e) {} }", "void foo() { try {} catch(Exception e) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 2);
    BOOST_TEST(catchData.parameters.at(0).IsDelete());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "foo bar|");
    BOOST_TEST(catchData.parameters.at(1).IsCommon());
    BOOST_TEST(catchData.parameters.at(1).ToString() == "Exception e");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common_param_delete_back) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(Exception e, foo bar) {} }", "void foo() { try {} catch(Exception e) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 2);
    BOOST_TEST(catchData.parameters.at(0).IsCommon());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "Exception e");
    BOOST_TEST(catchData.parameters.at(1).IsDelete());
    BOOST_TEST(catchData.parameters.at(1).ToString() == "foo bar|");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}

BOOST_AUTO_TEST_CASE(catch_common_param_replace) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher("void foo() { try {} catch(std::string str) {} }", "void foo() { try {} catch(Exception e) {} }");

    BOOST_TEST(runner.GetDeclStmtInfo().size()     == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.size() == 1);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->statements.at(0).OriginalType().name() == typeid(std::shared_ptr<srcDispatch::TryData>).name());

    const srcDispatch::TryData& tryData = *std::any_cast<std::shared_ptr<srcDispatch::TryData>>(runner.GetFunctionInfo().at(0)->block->statements.at(0).GetElement());
    BOOST_TEST(bool(tryData.block));
    BOOST_TEST(bool(tryData.block.IsCommon()));

    BOOST_TEST(tryData.clauses.size() == 1);
    BOOST_TEST(tryData.clauses.at(0).IsCommon());

    const srcDispatch::CatchData& catchData = *std::any_cast<std::shared_ptr<srcDispatch::CatchData>>(tryData.clauses.at(0).GetElement());
    BOOST_TEST(catchData.parameters.size() == 2);
    BOOST_TEST(catchData.parameters.at(0).IsDelete());
    BOOST_TEST(catchData.parameters.at(0).ToString() == "std::string str|");
    BOOST_TEST(catchData.parameters.at(1).IsInsert());
    BOOST_TEST(catchData.parameters.at(1).ToString() == "|Exception e");

    BOOST_TEST(catchData.block.IsCommon());

    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->labels.size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->cases.size()  == 0);
    BOOST_TEST(runner.GetFunctionInfo().at(0)->block->blocks.size() == 0);

    BOOST_TEST(runner.GetFunctionInfo().at(0).ToString() == "void foo() {}");
}
