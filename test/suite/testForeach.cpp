// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testForeach.cpp
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

#include <ForeachPolicy.hpp>

// Define test data
namespace data = boost::unit_test;
BOOST_AUTO_TEST_CASE(basic_foreach) {
    srcDispatch::DispatchRunner runner("C#");
    std::string src = R"(
    class Program {
        static void Main() {
            string[] fruits = { "Apple", "Banana", "Orange" };
            foreach (string fruit in fruits)
            {
                Console.WriteLine(fruit);
            }
        }
    }
    )";
    runner.RunDispatcher({{src,src}});

    BOOST_TEST(runner.GetClassInfo().size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.size() == 1);
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->name.ToString() == "Main");
    BOOST_TEST(runner.GetClassInfo().at(0)->methods.at(0)->block->statements.size() == 2);

    const srcDispatch::ForeachData& foreachData = *std::any_cast<std::shared_ptr<srcDispatch::ForeachData>>(
        runner.GetClassInfo().at(0)->methods.at(0)->block->statements.at(1).GetElement()
    );
    BOOST_TEST(foreachData.control->init);
    BOOST_TEST(foreachData.block->statements.size() == 1);

    const srcDispatch::DeclData& declData = *std::any_cast<std::shared_ptr<srcDispatch::DeclData>>(
        foreachData.control->init->inits.at(0).GetElement()
    );
    BOOST_TEST(declData.range);
}
