/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TAFunctions.cpp
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

#include "TAFunctions.h"

const string TAFunctions::SCHEMA = "//Generated TA File\n//Author: Jingwei Wu & Bryan J Muscedere\n\nSCHEME TUPLE :\nre"
        "ference\tcRoot\t\t\tcRoot\ncontain\t\tcRoot\t\t\tcRoot\n\n$INHERIT\tcArchitecturalNds\tcRoot\n$INHERIT\tcAsgNd"
        "s\t\t\tcRoot\n$INHERIT\tcSubSystem\t\tcArchitecturalNds\n$INHERIT\tcObjectFile\t\tcArchitecturalNds\n$INHERIT\t"
        "cFunction\t\tcAsgNds\n$INHERIT\tcObject\t\t\tcAsgNds\n\nSCHEME ATTRIBUTE :\n$ENTITY {\n\tx\n\ty\n\twidth\n\the"
        "ight\n\tlabel\n}\n\ncRoot {\n\telision = contain\n\tcolor = (0.0 0.0 0.0)\n\tfile\n\tline\n\tname\n}\n\ncAsgNd"
        "s {\n\tbeg\n\tend\n\tfile\n\tline\n\tvalue\n\tcolor = (0.0 0.0 0.0)\n}\n\ncArchitecturalNds {\n\tclass_style ="
        " 4\n\tcolor = (0.0 0.0 1.0)\n\tcolor = (0.0 0.0 0.0)\n}\n\ncSubSystem {\n\tclass_style = 4\n\tcolor = (0.0 0.0"
        " 1.0)\n}\n\ncObjectFile {\n\tclass_style = 4\n\tcolor = (0.6 0.8 0.6)\n\tlabelcolor = (0.0 0.0 0.0)\n}\n\ncFun"
        "ction {\n\tcolor = (1.0 0.0 0.0)\n\tlabelcolor = (0.0 0.0 0.0)\n}\n\n(reference) {\n\tcolor = (0.0 0.0 0.0)\n}"
        "\n\n";
const string TAFunctions::O_FILE_EXT = ".o";
const string TAFunctions::C_FILE_EXT = ".c";
const string TAFunctions::CPLUS_FILE_EXT = ".cc";
const string TAFunctions::CPLUSPLUS_FILE_EXT = ".cpp";

/**
 * Generates a TA file based on some output path.
 * Does this based on the graph that has been generated
 * since this function was called.
 * @param outputPath The output file path.
 * @return Return code indicating success.
 */
bool TAFunctions::generateTAFile(string outputPath, TAGraph* graph){
    //Create file pointer.
    std::ofstream taFile;
    taFile.open (outputPath.c_str());

    //Check if the file is opened.
    if (!taFile.is_open()){
        return false;
    }

    //First, we start by generating the schema.
    taFile << SCHEMA;

    //Next, we create the instance system.
    taFile << "FACT TUPLE :" << endl;
    taFile << graph->printInstances();

    //Next, we create the relationships.
    taFile << graph->printRelationships();

    //Finally, we print attribute information.
    taFile << endl << "FACT ATTRIBUTE :" << endl;
    taFile << graph->printAttributes();

    taFile.close();

    return true;
}

/**
 * Based on some start path, recursively finds all
 * C/C++ source files in the directories. To use, set first arg
 * as your path and second arg as NULL.
 * @param graph The TAGraph object that is being used to store project info.
 * @param printer The printer to print file processes.
 * @param curr The current path being looked at.
 * @param prev The previous path looked at.
 * @return Vector of object files.
 */
vector<path> TAFunctions::getSourceFiles(TAGraph* graph, PrintOperation parentPrint, path curr, path prev){
    //Generate a vector of extensions.
    vector<string> extVec = vector<string>();
    extVec.push_back(TAFunctions::C_FILE_EXT);
    extVec.push_back(TAFunctions::CPLUS_FILE_EXT);
    extVec.push_back(TAFunctions::CPLUSPLUS_FILE_EXT);

    //Runs the get file function.
    vector<path> files = getFiles(curr, prev, extVec, parentPrint);

    //Process the files.
    addFiles(graph, files);

    return files;
}

/**
 * Based on some start path, recursively finds all
 * O files in the directories. To use, set first arg
 * as your path and second arg as NULL.
 * @param graph The TAGraph object that is being used to store project info.
 * @param printer The printer to print file processes.
 * @param curr The current path being looked at.
 * @param prev The previous path looked at.
 * @return Vector of object files.
 */
vector<path> TAFunctions::getObjectFiles(TAGraph* graph, PrintOperation parentPrint, path curr, path prev){
    //Generate a vector of extensions.
    vector<string> extVec = vector<string>();
    extVec.push_back(TAFunctions::O_FILE_EXT);

    //Runs the get file function.
    vector<path> files = getFiles(curr, prev, extVec, parentPrint);

    //Process the files.
    addFiles(graph, files);

    return files;
}

/**
 * Based on some start path, recursively finds all
 * O files in the directories. To use, set first arg
 * as your path and second arg as NULL.
 * @param graph The TAGraph object that is being used to store project info.
 * @param curr The current path being looked at.
 * @param prev The previous path looked at.
 * @param ext Vector of extensions to look for.
 * @param printer Print methods for printing details.
 * @return Vector of object files.
 */
vector<path> TAFunctions::getFiles(path curr, path prev, vector<string> ext, PrintOperation printer){
    vector<path> interiorDir = vector<path>();
    vector<path> files = vector<path>();
    directory_iterator endIter;

    //Start by iterating through and inspecting each file.
    for (directory_iterator iter(curr); iter != endIter; iter++){
        //Check what the current file is.
        if (is_regular_file(iter->path())){
            //Check the extension.
            string extFile = extension(iter->path());

            //Iterates through the extension vector.
            for (int i = 0; i < ext.size(); i++){
                //Checks the file.
                if (extFile.compare(ext.at(i)) == 0){
                    files.push_back(iter->path());
                    printer.printFileFound(iter->path().filename().string());
                }
            }
        } else if (is_directory(iter->path())){
            //Add the directory to the search system.
            interiorDir.push_back(iter->path());
        }
    }

    //Next, goes through all the sub-directories.
    for (int i = 0; i < interiorDir.size(); i++){
        //Gets the path and object files.
        path current = interiorDir.at(i);
        vector<path> newObj = getFiles(current, curr, ext, printer);

        //Adds to current vector.
        files.insert(files.end(), newObj.begin(), newObj.end());
    }

    //Return a list of object files.
    return files;
}

/**
 * Adds files to the TAGraph object. This ensures
 * that only paths containing object files are added.
 * @param graph The TA graph being used.
 * @param files A list of object files being processed.
 */
void TAFunctions::addFiles(TAGraph* graph, vector<path> files){
    //Iterate through each of the files.
    for (path current : files){
        string fileName = current.filename().string();
        string previous = "";

        //Now, iterates through the path.
        for (auto const& pItem : current){
            //Create the pItem node.
            if (fileName.compare(pItem.string()) != 0){
                graph->addNode(pItem.string(), BFXNode::SUBSYSTEM, "", "");
            } else {
                graph->addNode(pItem.string(), BFXNode::FILE, "", "");
            }


            //Check if we add the edge.
            if (previous.compare("") != 0){
                if (!graph->doesContainEdgeExist(previous, pItem.string()))
                    graph->addEdge(previous, pItem.string(), BFXEdge::CONTAINS);
            }

            previous = pItem.string();
        }
    }
}
