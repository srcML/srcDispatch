#ifndef SRCDISPATCHTEST_HEADER
#define SRCDISPATCHTEST_HEADER

#include <iostream>
#include <srcml.h>

#include <srcDispatcher.hpp>
#include <srcSAXHandler.hpp>
#include <IfStmtPolicySingleEvent.hpp>
#include <SwitchPolicySingleEvent.hpp>
#include <WhilePolicySingleEvent.hpp>
#include <ForPolicySingleEvent.hpp>
#include <DoPolicySingleEvent.hpp>
#include <FunctionPolicySingleEvent.hpp>
#include <ClassPolicySingleEvent.hpp>
#include <UnitPolicySingleEvent.hpp>

enum class LANG {
    C, CXX, CSHARP, JAVA
};

void Print_Ok(std::string msg);
void Print_Info(std::string msg);
void Print_Err(std::string msg);

// Converts source code to srcML based on language option (C, C++, C-Sharp, Java)
std::string StringToSrcML(std::string str, const char* fileName, LANG lang = LANG::CXX);

#endif