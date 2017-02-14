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

#ifndef BFX64_TAFUNCTIONS_H
#define BFX64_TAFUNCTIONS_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <sstream>
#include <map>
#include <fstream>
#include "Graph/TAGraph.h"
#include "Print/PrintOperation.h"

using namespace std;
using namespace boost::filesystem;

class TAFunctions {
public:
    /** Processing Functions */
    static bool generateTAFile(string outputPath, TAGraph* graph);
    static bool startTAGeneration(string outputPath);
    static bool dumpTAFile(TAGraph* graph);
    static void endTAFile();

    static vector<path> getSourceFiles(TAGraph* graph, PrintOperation parentPrint, path curr, path prev);
    static vector<path> getObjectFiles(TAGraph* graph, PrintOperation parentPrint, path curr, path prev);

private:
    /** TA Schema */
    static const string SCHEMA;

    /** File Extensions */
    static const string O_FILE_EXT;
    static const string C_FILE_EXT;
    static const string CPLUS_FILE_EXT;
    static const string CPLUSPLUS_FILE_EXT;

    /** Private Recursive Helper Methods */
    static vector<path> getFiles(path curr, path prev, vector<string> ext, PrintOperation printer);
    static void addFiles(TAGraph* graph, vector<path> files);

    /** TA Dump OStream */
    static std::ofstream taDump;
};

#endif //BFX64_TAFUNCTIONS_H
