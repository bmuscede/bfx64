/////////////////////////////////////////////////////////////////////////////////////////////////////////
// main.cpp
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Main method is contained in here. Processes command line arguments
// then passes off program control to ElfReader. This program generates
// a Tuple-Attribute file based on a slew of object files. It can handle
// object files in 32bit or 64bit compiled using the latest ELF standard.
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

#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
#include "ELF/ElfReader.h"

using namespace std;
namespace po = boost::program_options;

/** Default Output Directory */
const std::string DEFAULT_OUT = "./out.ta";

/** Program String */
const std::string DEFAULT_MSG = "Usage: bfx64 [args]"
        "\nUniversity of Waterloo, 2016\nBryan J Muscedere\n\n"
        "Information:\n"
        "Extracts a series of abstract facts from C/C++ programs to allow for a concise,\n"
        "detailed, and whole-system representation of a software project.\nGenerates a Tuple-Attribute file"
        " based on the facts collected.\n\nArguments";

/**
 * Main driver method for the program. Takes in command line
 * arguments using boost and then calls the ElfReader to
 * find all object files in subdirectories.
 * @param argc The number of arguments.
 * @param argv Vector of arguments.
 * @return Return code.
 */
int main(int argc, const char *argv[]) {
    bool suppressFlag;
    bool verboseFlag;

    //Sets up the program options.
    po::options_description desc(DEFAULT_MSG);
    desc.add_options()
            ("help,h", "Show the help message.")
            ("dir,d", po::value<string>()->default_value(""), "Sets the starting directory to search for files.")
            ("out,o", po::value<string>()->default_value(DEFAULT_OUT), "Sets the output file (instead of out.ta).")
            ("suppress,s", po::bool_switch(&suppressFlag), "Stops bfx64 from searching for object files.")
            ("object,i", po::value<vector<string>>(), "Adds an object file for bfx64 to process.")
            ("exclude,e", po::value<vector<string>>(), "Removes an object file from bfx64's processing queue.")
            ("verbose,v", po::bool_switch(&verboseFlag), "Sets verbose output for bfx64.");
            ;

    //Creates a variable map.
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    } catch (const std::exception& ex) {
        cerr << "Error: " <<  ex.what() << endl << endl;
        cout << desc << endl;
        return 1;
    }
    po::notify(vm);

    //Check for help.
    if (vm.count("help")){
        cout << desc << endl;
        return 1;
    }

    //Sets the default parameters.
    string startingDir = vm["dir"].as<string>();
    string output = vm["out"].as<string>();

    //Gets the input and output args.
    vector<string> inputFiles;
    vector<string> outputFiles;
    try {
        inputFiles = vm["object"].as<vector<string>>();
    } catch (...) {
        inputFiles = vector<string>();
    }
    try {
        outputFiles = vm["exclude"].as<vector<string>>();
    } catch (...) {
        outputFiles = vector<string>();
    }

    //Starts theo ELFReader.
    ElfReader reader = ElfReader(startingDir, output, suppressFlag, verboseFlag);
    reader.read(inputFiles, outputFiles);

    return 0;
}