/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ElfReader.h
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Driver class that inspects object files located in a specific directory.
// From that, it generates a graph showing how objects and functions interact
// and call each other. From it, can output the graph in Tuple-Attribute format.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BFX64_ELFREADER_H
#define BFX64_ELFREADER_H

#include <boost/filesystem.hpp>
#include <elfio/elfio.hpp>
#include <string>
#include <map>
#include "../Graph/TAGraph.h"

class ElfReader {
public:
    /** Constructor / Destructor */
    ElfReader(std::string startDir, std::string outPath);
    ~ElfReader();

    /** Generation Method */
    void read();

private:
    /** Private Variables */
    std::map<std::string, std::vector<std::string>> externalRef;
    const std::string REL_PREFIX = ".rel";
    std::string outputDirectory;
    boost::filesystem::path curPath;
    TAGraph* graph;

    /** Helper Methods to Read */
    void process(boost::filesystem::path objectFile);
    void processSymbolTable(boost::filesystem::path oFile, ELFIO::section* symTab);
    void resolveReferences(boost::filesystem::path oFile, ELFIO::section* symTab);
    void processUndefinedReferences();

    /** Sub Helper Methods */
    std::string generateID(std::string path, ELFIO::Elf_Half sectionNum, ELFIO::Elf64_Addr addr);
    std::string demangleName(const char* mangledName);
    int getRelocationSection(std::string path, ELFIO::Elf_Half secNum);
    bool isValidReloc(ELFIO::Elf64_Addr startPos, ELFIO::Elf64_Addr endPos, ELFIO::Elf64_Addr relocPos);
};

#endif //BFX64_ELFREADER_H
