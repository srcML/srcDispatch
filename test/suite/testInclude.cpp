// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file testInclude.cpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the srcDiff Infrastructure.
 */

#define BOOST_TEST_MODULE include tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(common_include) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"file.hpp\"", "#include \"file.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative);
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"file.hpp\"");
}

BOOST_AUTO_TEST_CASE(common_multi_include) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"file.hpp\"\n#include \"include.hpp\"", "#include \"file.hpp\"\n#include \"include.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 2);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative);
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetElement() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"file.hpp\"");

    BOOST_TEST(runner.GetIncludeInfo().at(1).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(1)->isRelative.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(1)->isRelative);
    BOOST_TEST(runner.GetIncludeInfo().at(1)->path.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(1)->path.GetElement() == "include.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(1).ToString() == "#include \"include.hpp\"");
}

BOOST_AUTO_TEST_CASE(delete_include) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"file.hpp\"", ""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsDelete());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsDelete());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative);
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsDelete());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetElement() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"file.hpp\"|");
}

BOOST_AUTO_TEST_CASE(insert_include) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "#include \"file.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsInsert());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsInsert());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative);
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsInsert());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetElement() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "|#include \"file.hpp\"");
}

BOOST_AUTO_TEST_CASE(change_relativity) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"file.hpp\"", "#include <file.hpp>"}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsChange());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.GetOriginal());
    BOOST_TEST(!runner.GetIncludeInfo().at(0)->isRelative.GetModified());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetElement() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"file.hpp\"|#include <file.hpp>");
}

BOOST_AUTO_TEST_CASE(change_relativity_revese) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include <file.hpp>", "#include \"file.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsChange());
    BOOST_TEST(!runner.GetIncludeInfo().at(0)->isRelative.GetOriginal());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.GetModified());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetElement() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include <file.hpp>|#include \"file.hpp\"");
}

BOOST_AUTO_TEST_CASE(change_filename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"file.hpp\"", "#include \"include.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsChange());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetOriginal() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetModified() == "include.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"file.hpp\"|#include \"include.hpp\"");
}

BOOST_AUTO_TEST_CASE(change_filename_reverse) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"include.hpp\"", "#include \"file.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsChange());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetOriginal() == "include.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetModified() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"include.hpp\"|#include \"file.hpp\"");
}

BOOST_AUTO_TEST_CASE(change_relativity_and_filename) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include \"file.hpp\"", "#include <include.hpp>"}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsChange());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.GetOriginal());
    BOOST_TEST(!runner.GetIncludeInfo().at(0)->isRelative.GetModified());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsChange());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetOriginal() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetModified() == "include.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include \"file.hpp\"|#include <include.hpp>");
}

BOOST_AUTO_TEST_CASE(change_relativity_and_filename_reverse) {

    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"#include <include.hpp>", "#include \"file.hpp\""}});

    BOOST_TEST(runner.GetIncludeInfo().size()  == 1);
    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 0);

    BOOST_TEST(runner.GetIncludeInfo().at(0).IsCommon());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.IsChange());
    BOOST_TEST(!runner.GetIncludeInfo().at(0)->isRelative.GetOriginal());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->isRelative.GetModified());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.IsChange());
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetOriginal() == "include.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0)->path.GetModified() == "file.hpp");
    BOOST_TEST(runner.GetIncludeInfo().at(0).ToString() == "#include <include.hpp>|#include \"file.hpp\"");
}
