//
// Created by bmuscede on 19/09/16.
//

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

    static vector<path> getFiles(path curr, path prev, vector<string> ext);
    static void addFiles(TAGraph* graph, vector<path> files);
};

#endif //BFX64_TAFUNCTIONS_H
