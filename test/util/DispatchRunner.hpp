// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DispatchRunner.hpp
 *
 * @copyright Copyright (C) 2025-2025 SDML (www.srcML.org)
 *
 * This file is part of the Dispatch Infrastructure.
 */

#ifndef INCLUDED_DISPATCH_RUNNER_HPP
#define INCLUDED_DISPATCH_RUNNER_HPP

#include <srcSAXController.hpp>
#include <srcDispatchUtilities.hpp>
#include <srcDispatcher.hpp>

#include <UnitPolicy.hpp>
#include <DeltaElement.hpp>

#include <fstream>
#include <filesystem>

namespace srcDispatch {

typedef std::pair<std::string, std::string> SourcePair;

class DispatchRunner : public srcDispatch::PolicyListener  {
private:

    void writeFile(const std::string& filename, const std::string& contents) {
        std::ofstream file(filename);
        file << contents;
    }

    std::string readDiffFile() {
        std::ifstream srcDiffFile("srcdiff.xml", std::ios::ate);
        std::size_t size = srcDiffFile.tellg();
        srcDiffFile.seekg(0);

        std::string srcDiffStr(size, '\0');
        srcDiffFile.read(&srcDiffStr[0], size);
        return srcDiffStr;
    }

    std::string srcDiff(const std::string& original, const std::string& modified) {

        static const std::string original_filename_base = "original";
        static const std::string modified_filename_base = "modified";

        std::string original_filename = original_filename_base + extension;
        std::string modified_filename = modified_filename_base + extension;

        writeFile(original_filename, original);
        writeFile(modified_filename, modified);

        const std::string command = "srcdiff " + original_filename + ' ' + modified_filename + " -o srcdiff.xml";
        std::system(command.c_str());

        std::string srcDiffStr = readDiffFile();

        std::remove(original_filename.c_str());
        std::remove(modified_filename.c_str());
        std::remove("srcdiff.xml");

        return srcDiffStr;
    }

    std::string srcDiff(const std::vector<SourcePair>& sourcePairs) {

        std::filesystem::create_directory("original");
        std::filesystem::create_directory("modified");

        int pairNum = 0;
        for(const SourcePair& pair : sourcePairs) {
            std::string numStr = std::to_string(pairNum);
            writeFile("original/file" + numStr + extension, pair.first);
            writeFile("modified/file" + numStr + extension, pair.second);
            ++pairNum;
        }

        std::system("srcdiff original modified -o srcdiff.xml");

        std::string srcDiffStr = readDiffFile();

        std::filesystem::remove_all("original");
        std::filesystem::remove_all("modified");
        std::remove("srcdiff.xml");

        return srcDiffStr;
    }

    std::shared_ptr<UnitData> unit;

public:
    DispatchRunner(const std::string& language = "C++") : extension() {
        static const std::unordered_map<std::string, std::string> languageToExtension
         = {
                { "C++",  ".cpp"},
                { "C",    ".c"},
                { "Java", ".java"},
                { "C#",   ".cs"},
           };

        extension = languageToExtension.at(language);

    }

    const std::vector<DeltaElement<std::shared_ptr<IncludeData>>>& GetIncludeInfo() const {
        return unit->includes;
    }

    const std::vector<DeltaElement<std::shared_ptr<DeclStmtData>>>& GetDeclStmtInfo() const {
        return unit->declStmts;
    }

    const std::vector<DeltaElement<std::shared_ptr<ClassData>>>& GetClassInfo() const {
        return unit->classes;
    }

    const std::vector<DeltaElement<std::shared_ptr<FunctionData>>>& GetFunctionInfo() const {
        return unit->functions;
    }

    const std::vector<DeltaElement<std::shared_ptr<ClassDeclData>>>& GetClassDeclInfo() const {
        return unit->classDecls;
    }

    void Notify(const srcDispatch::PolicyDispatcher * policy,
                const srcDispatch::srcSAXEventContext & ctx) override {
        unit = policy->Data<UnitData>();
    }

    void NotifyWrite(const srcDispatch::PolicyDispatcher* policy [[maybe_unused]], srcDispatch::srcSAXEventContext& ctx [[maybe_unused]]) override {}

    void RunDispatcher(const std::vector<SourcePair>& sourcePairs) {

        std::string srcDiffStr;
        if(sourcePairs.size() == 1) {
            srcDiffStr = srcDiff(sourcePairs.at(0).first, sourcePairs.at(0).second);
        } else {
            srcDiffStr = srcDiff(sourcePairs);        
        }

        try {
            srcSAXController control(srcDiffStr);
            srcDispatch::srcDispatcher<srcDispatch::UnitPolicy> dispatch(this);
            control.parse(&dispatch); //Start parsing
        } catch(SAXError error) {
            std::cerr << error.message;
            exit(1);
        }
    }

private:
    std::string extension;
};

}

#endif