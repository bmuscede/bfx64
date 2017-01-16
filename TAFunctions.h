/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TAFunctions.h
//
// Created By: Bryan J Muscedere
// Date: 19/09/16.
//
// Catch all static source file containing a collection
// of important processing functions. These allow you to
// write TA graphs, read file paths, and find object and
// source files.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BFX64_TAFUNCTIONS_H
#define BFX64_TAFUNCTIONS_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <sstream>
#include <map>
#include <fstream>
#include "Graph/TAGraph.h"

using namespace std;
using namespace boost::filesystem;

class TAFunctions {
public:
    /** Processing Functions */
    static void generateTAFile(string outputPath, TAGraph* graph);
    static vector<path> getSourceFiles(TAGraph* graph, path curr, path prev);
    static vector<path> getObjectFiles(TAGraph* graph, path curr, path prev);

private:
    /** TA Schema */
    static const string SCHEMA;

    /** File Extensions */
    static const string O_FILE_EXT;
    static const string C_FILE_EXT;
    static const string CPLUS_FILE_EXT;
    static const string CPLUSPLUS_FILE_EXT;

    /** Private Recursive Helper Methods */
    static vector<path> getFiles(path curr, path prev, vector<string> ext);
    static void addFiles(TAGraph* graph, vector<path> files);
};

#endif //BFX64_TAFUNCTIONS_H
