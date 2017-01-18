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
#include "ELF/ElfReader.h"

using namespace std;
namespace po = boost::program_options;

/** Default Output Directory */
const std::string DEFAULT_OUT = "./out.ta";

/** Program String */
const std::string DEFAULT_MSG = "Usage: bfx64 [args]\nUniversity of Waterloo, 2016\nBryan J Muscedere\n\n"
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
    //Sets up the program options.
    po::options_description desc(DEFAULT_MSG);
    desc.add_options()
            ("help,h", "Show the help message.")
            ("dir,d", po::value<string>()->default_value("."), "Sets the starting directory to search for files.")
            ("out,o", po::value<string>()->default_value(DEFAULT_OUT), "Sets the output file (instead of out.ta).")
            ;

    //Creates a variable map.
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //Check for help.
    if (vm.count("help")){
        cout << desc << endl;
        return 1;
    }

    //Sets the default parameters.
    string startingDir = vm["dir"].as<string>();
    string output = vm["out"].as<string>();

    //Starts theo ELFReader.
    ElfReader reader = ElfReader(startingDir, output);
    reader.read();

    return 0;
}