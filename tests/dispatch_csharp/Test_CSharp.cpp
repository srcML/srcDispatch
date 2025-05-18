#include "../srcDispatchTest.hpp"

class SrcDispatchTest
        : public srcDispatch::EventListener,
          public srcDispatch::PolicyDispatcher,
          public srcDispatch::PolicyListener {
public:
    ~SrcDispatchTest() {};
    
    // Use string srcml buffer ctor of srcSAXController
    SrcDispatchTest(const std::string& sourceCodeStr, std::initializer_list<srcDispatch::PolicyListener *> listeners = {})
            : srcDispatch::PolicyDispatcher(listeners) {
        srcSAXController control(sourceCodeStr);
        srcDispatch::srcDispatcherSingleEvent<UnitPolicy> handler(this);
        control.parse(&handler); // Start parsing
    }

    void Notify(const PolicyDispatcher *policy, const srcDispatch::srcSAXEventContext &ctx) override {
        if(typeid(ClassPolicy) == typeid(*policy)) {
            Print_Info("Class Data Recieved!");
            classDataCollection.push_back(policy->Data<ClassData>());
        } else if(typeid(FunctionPolicy) == typeid(*policy)) {
            Print_Info("Function Data Recieved!");
        } else if (typeid(DeclTypePolicy) == typeid(*policy)) {
            Print_Info("Potential Global Data Recieved!");
        }
    }

    void NotifyWrite(const PolicyDispatcher *policy [[maybe_unused]], srcDispatch::srcSAXEventContext &ctx [[maybe_unused]]) {}

    std::vector<std::shared_ptr<ClassData>> classDataCollection;

protected:
    std::any DataInner() const override {
        return (void *) 0; // export profile to listeners
    }
};

// Basic test to check if dispatcher collects basic info about C# Classes
// and the classes methods
void Test_One() {
    Print_Info("Starting Test 1!");
    std::string sourceCodeStr = "using System;\n"
                                "public class HelloWorld {\n"
                                //"    void lorem() {\n"
                                //"        Console.WriteLine(\"Lorem!\");\n"
                                //"    }\n"
                                "    public static void Main(string[] args) {\n"
                                "        Console.WriteLine(\"HelloWorld!\");\n"
                                //"        lorem();\n"
                                "    }\n"
                                "}";
    std::string srcmlStr = StringToSrcML(sourceCodeStr, "test.cs", LANG::CSHARP);

    SrcDispatchTest dispatch(srcmlStr); // store the results

    std::cout << "-------------------------------\n";
    std::cout << sourceCodeStr << std::endl;
    std::cout << "-------------------------------\n";

    // access the results and perform testing
    if (!(dispatch.classDataCollection.size() == 1)) {
        Print_Info("No Class Data was Collected!");
        Print_Err("Test Failed!");
    }

    if (!(dispatch.classDataCollection.back()->name->ToString() == "HelloWorld")) {
        Print_Info("Name of Class is Incorrect!");
        Print_Err("Test Failed!");
    }

    // 0 index is public accessor
    if (!(dispatch.classDataCollection.back()->methods[0].size() == 1)) {
        Print_Info("Missing Class Public Method Data!");
        Print_Err("Test Failed!");
    }

    if (!(dispatch.classDataCollection.back()->methods[0].back()->name->ToString() == "Main")) {
        Print_Info("Name of Class Method is Incorrect!");
        Print_Err("Test Failed!");
    }

    Print_Ok("Test Passed!");
}

// Test non-public class method collecting
void Test_Two() {
    Print_Info("Starting Test 2!");
    std::string sourceCodeStr = "using System;\n"
                                "public class HelloWorld {\n"
                                "    void lorem() {\n"
                                "        Console.WriteLine(\"Lorem!\");\n"
                                "    }\n"
                                "    public static void Main(string[] args) {\n"
                                "        Console.WriteLine(\"HelloWorld!\");\n"
                                "        lorem();\n"
                                "    }\n"
                                "}";
    std::string srcmlStr = StringToSrcML(sourceCodeStr, "test.cs", LANG::CSHARP);

    SrcDispatchTest dispatch(srcmlStr); // store the results

    std::cout << "-------------------------------\n";
    std::cout << sourceCodeStr << std::endl;
    std::cout << "-------------------------------\n";

    /*
        enum AccessSpecifier { PUBLIC = 0, PRIVATE = 1, PROTECTED = 2 };
    */
    int accessor_specifier = 1;

    if (!(dispatch.classDataCollection.back()->methods[accessor_specifier].size() == 1)) {
        Print_Info("Error with Class Method Data!");

        int publicCount = dispatch.classDataCollection.back()->methods[0].size();
        int protectedCount = dispatch.classDataCollection.back()->methods[1].size();
        int privateCount = dispatch.classDataCollection.back()->methods[2].size();

        Print_Info("Number of Public Methods -> " + std::to_string(publicCount));
        Print_Info("Number of Protected Methods -> " + std::to_string(protectedCount));
        Print_Info("Number of Private Methods -> " + std::to_string(privateCount));

        Print_Err("Test Failed!");
    }

    if (!(dispatch.classDataCollection.back()->methods[accessor_specifier].back()->name->ToString() == "lorem")) {
        Print_Info("Name of Class Method is Incorrect!");
        Print_Err("Test Failed!");
    }

    Print_Ok("Test Passed!");
}

int main() {
    Print_Info("Starting C# Tests. . .");
    Test_One();
    Test_Two();
    Print_Ok("All Tests Passed!");
}