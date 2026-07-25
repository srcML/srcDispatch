
#define BOOST_TEST_MODULE class_and_struct tests
#include <boost/test/included/unit_test.hpp>

#include <DispatchRunner.hpp>

#include <NamePolicy.hpp>
#include <LiteralPolicy.hpp>
#include <OperatorPolicy.hpp>

// Define test data
namespace data = boost::unit_test;

BOOST_AUTO_TEST_CASE(typedecl_class_common) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo;", "class foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsCommon());
    BOOST_TEST(classDeclData->type.IsCommon());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::CLASS_DECL);

    BOOST_TEST(classDeclData->name.IsCommon());
    BOOST_TEST(classDeclData->name.ToString() == "foo");
}

BOOST_AUTO_TEST_CASE(typedecl_enum_common) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"enum foo;", "enum foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsCommon());
    BOOST_TEST(classDeclData->type.IsCommon());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::ENUM_DECL);

    BOOST_TEST(classDeclData->name.IsCommon());
    BOOST_TEST(classDeclData->name.ToString() == "foo");
}

BOOST_AUTO_TEST_CASE(typedecl_struct_common) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"struct foo;", "struct foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsCommon());
    BOOST_TEST(classDeclData->type.IsCommon());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::STRUCT_DECL);

    BOOST_TEST(classDeclData->name.IsCommon());
    BOOST_TEST(classDeclData->name.ToString() == "foo");
}

BOOST_AUTO_TEST_CASE(typedecl_union_common) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"union foo;", "union foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsCommon());
    BOOST_TEST(classDeclData->type.IsCommon());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::UNION_DECL);

    BOOST_TEST(classDeclData->name.IsCommon());
    BOOST_TEST(classDeclData->name.ToString() == "foo");
}

BOOST_AUTO_TEST_CASE(typedecl_class_insert) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "class foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsInsert());
    BOOST_TEST(classDeclData->type.IsInsert());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::CLASS_DECL);

    BOOST_TEST(classDeclData->name.IsInsert());
    BOOST_TEST(classDeclData->name.ToString() == "|foo");
}

BOOST_AUTO_TEST_CASE(typedecl_enum_insert) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "enum foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsInsert());
    BOOST_TEST(classDeclData->type.IsInsert());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::ENUM_DECL);

    BOOST_TEST(classDeclData->name.IsInsert());
    BOOST_TEST(classDeclData->name.ToString() == "|foo");
}

BOOST_AUTO_TEST_CASE(typedecl_struct_insert) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "struct foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsInsert());
    BOOST_TEST(classDeclData->type.IsInsert());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::STRUCT_DECL);

    BOOST_TEST(classDeclData->name.IsInsert());
    BOOST_TEST(classDeclData->name.ToString() == "|foo");
}

BOOST_AUTO_TEST_CASE(typedecl_union_insert) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"", "union foo;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsInsert());
    BOOST_TEST(classDeclData->type.IsInsert());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::UNION_DECL);

    BOOST_TEST(classDeclData->name.IsInsert());
    BOOST_TEST(classDeclData->name.ToString() == "|foo");
}

BOOST_AUTO_TEST_CASE(typedecl_class_delete) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo;", ""}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsDelete());
    BOOST_TEST(classDeclData->type.IsDelete());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::CLASS_DECL);

    BOOST_TEST(classDeclData->name.IsDelete());
    BOOST_TEST(classDeclData->name.ToString() == "foo|");
}

BOOST_AUTO_TEST_CASE(typedecl_enum_delete) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"enum foo;", ""}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsDelete());
    BOOST_TEST(classDeclData->type.IsDelete());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::ENUM_DECL);

    BOOST_TEST(classDeclData->name.IsDelete());
    BOOST_TEST(classDeclData->name.ToString() == "foo|");
}

BOOST_AUTO_TEST_CASE(typedecl_struct_delete) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"struct foo;", ""}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsDelete());
    BOOST_TEST(classDeclData->type.IsDelete());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::STRUCT_DECL);

    BOOST_TEST(classDeclData->name.IsDelete());
    BOOST_TEST(classDeclData->name.ToString() == "foo|");
}

BOOST_AUTO_TEST_CASE(typedecl_union_delete) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"union foo;", ""}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsDelete());
    BOOST_TEST(classDeclData->type.IsDelete());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::UNION_DECL);

    BOOST_TEST(classDeclData->name.IsDelete());
    BOOST_TEST(classDeclData->name.ToString() == "foo|");
}

BOOST_AUTO_TEST_CASE(typedecl_class_rename) {
    srcDispatch::DispatchRunner runner;
    runner.RunDispatcher({{"class foo;", "class bar;"}});

    BOOST_TEST(runner.GetDeclStmtInfo().size() == 0);
    BOOST_TEST(runner.GetFunctionInfo().size() == 0);
    BOOST_TEST(runner.GetClassInfo().size()    == 1);

    const srcDispatch::DeltaElement<std::shared_ptr<srcDispatch::ClassData>>& classDeclData = runner.GetClassInfo().at(0); 
    BOOST_TEST(classDeclData.IsCommon());
    BOOST_TEST(classDeclData->type.IsCommon());
    BOOST_TEST(classDeclData->type.GetElement() == srcDispatch::ClassData::CLASS_DECL);

    BOOST_TEST(classDeclData->name.IsCommon());
    BOOST_TEST(classDeclData->name.ToString() == "foo|bar");
}
