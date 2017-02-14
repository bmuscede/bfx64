/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ElfReader.h
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Driver class that inspects object files located in a specific directory.
// From that, it generates a graph showing how objects and functions interact
// and call each other. From it, can output the graph in Tuple-Attribute format.
//
// Copyright (C) 2017, Bryan J. Muscedere
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BFX64_ELFREADER_H
#define BFX64_ELFREADER_H

#include <boost/filesystem.hpp>
#include <elfio/elfio.hpp>
#include <string>
#include <map>
#include "../Graph/TAGraph.h"
#include "../Print/PrintOperation.h"

class ElfReader {
public:
    /** Constructor / Destructor */
    ElfReader(std::string startDir, std::string outPath, bool suppress, bool verbose, bool lowMemory = false);
    ~ElfReader();

    /** Generation Method */
    void read(std::vector<std::string> insertFiles, std::vector<std::string> removeFiles);

private:
    /** Private Variables */
    std::map<std::string, std::vector<std::string>> externalRef;
    const std::string REL_PREFIX[2] = {".rel", ".rela"};
    std::string outputDirectory;
    boost::filesystem::path curPath;
    bool suppress;
    TAGraph* graph;
    PrintOperation printer;
    bool lowMem;

    /** Helper Methods to Read */
    void process(boost::filesystem::path objectFile);
    void processSymbolTable(boost::filesystem::path oFile, ELFIO::section* symTab);
    void resolveReferences(boost::filesystem::path oFile, ELFIO::section* symTab);
    void processUndefinedReferences();

    /** Sub Helper Methods */
    std::string generateID(std::string path, ELFIO::Elf_Half sectionNum, ELFIO::Elf64_Addr addr, bool& success);
    std::string demangleName(const char* mangledName);
    int getRelocationSection(std::string path, ELFIO::Elf_Half secNum);
    bool isValidReloc(ELFIO::Elf64_Addr startPos, ELFIO::Elf64_Addr endPos, ELFIO::Elf64_Addr relocPos);
};

#endif //BFX64_ELFREADER_H
