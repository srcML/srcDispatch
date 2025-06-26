// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcDiffDispatchRunner.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcDiff.org)
 *
 * This file is part of the srcDiffDispatch Infrastructure.
 */

#ifndef INCLUDED_SRCDIFF_DISPATCH_RUNNER_HPP
#define INCLUDED_SRCDIFF_DISPATCH_RUNNER_HPP


#include <srcSAXController.hpp>
#include <srcDispatchUtilities.hpp>
#include <srcDispatcherSingleEvent.hpp>

#include <srcDiffUnitPolicy.hpp>
#include <DeltaElement.hpp>

#include <fstream>

namespace srcDiffDispatch {

class srcDiffDispatchRunner : public srcDispatch::PolicyListener  {
private:
    std::string srcDiff(const std::string& original, const std::string& modified) {

        std::ofstream originalFile("original.cpp");
        originalFile << original;
        originalFile.close();

        std::ofstream modifiedFile("modified.cpp");
        modifiedFile << modified;
        modifiedFile.close();

        std::system("srcdiff original.cpp modified.cpp -o srcdiff.xml");

        std::ifstream srcDiffFile("srcdiff.xml", std::ios::ate);
        std::size_t size = srcDiffFile.tellg();
        srcDiffFile.seekg(0);

        std::string srcDiffStr(size, '\0');
        srcDiffFile.read(&srcDiffStr[0], size);

        std::remove("original.cpp");
        std::remove("modified.cpp");
        std::remove("srcdiff.xml");

        return srcDiffStr;
    }

    std::shared_ptr<UnitData> unit;

public:
    srcDiffDispatchRunner() {}

    const std::vector<DeltaElement<std::shared_ptr<ClassData>>>& GetClassInfo() const {
        return unit->classInfo;
    }

    const std::vector<DeltaElement<std::shared_ptr<FunctionData>>>& GetFunctionInfo() const {
        return unit->functionInfo;
    }

    const std::vector<DeltaElement<std::shared_ptr<DeclStmtData>>>& GetDeclStmtInfo() const {
        return unit->declStmtInfo;
    }

    void Notify(const srcDispatch::PolicyDispatcher * policy,
                const srcDispatch::srcSAXEventContext & ctx) override {
        unit = policy->Data<UnitData>();
    }

    void NotifyWrite(const srcDispatch::PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}


    void RunDispatcher(const std::string& original, const std::string& modified) {

        std::string srcDiffStr = srcDiff(original, modified);
        try {
            srcSAXController control(srcDiffStr);
            srcDispatch::srcDispatcherSingleEvent<srcDiffDispatch::UnitPolicy> dispatch(this);
            control.parse(&dispatch); //Start parsing
        } catch(SAXError error) {
            std::cerr << error.message;
            exit(1);
        }
    }

};

}

#endif