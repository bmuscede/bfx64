/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ElfReader.cpp
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

#include <iostream>
#include <boost/filesystem.hpp>
#include <elfio/elfio.hpp>
#include <sstream>
#include <map>
#include "ElfReader.h"
#include "../TAFunctions.h"

using namespace std;
using namespace ELFIO;
using namespace boost::filesystem;

/**
 * Constructor that creates an ElfReader based on a start
 * directory and an output file path for the TA file.
 * @param startDir The start directory for O files.
 * @param outputPath The output location for the TA file.
 * @param suppress Whether we suppress looking for a file.
 */
ElfReader::ElfReader(string startDir, string outputPath, bool suppress){
    //Check if we have an empty directory.
    if (startDir.compare("") == 0){
        curPath = initial_path();
    } else {
        curPath = path(startDir);
    }

    //Next, generate the output directory.
    this->outputDirectory = outputPath;

    //Set suppress.
    this->suppress = suppress;
}

/**
 * Destructor that deletes the graph.
 */
ElfReader::~ElfReader(){
    delete graph;
}

/**
 * Driver method for the TA creation. Reads all O files and
 * then generates the TA file.
 */
void ElfReader::read(vector<string> inputFiles, vector<string> removeFiles){
    externalRef = map<string, vector<string>>();

    //Generate a new instance of the graph.
    graph = new TAGraph();

    //Start by reading all the files.
    vector<path> objectFiles;
    for (string file : inputFiles){
        path curr = path(file);

        //Check if the file exists.
        if (!boost::filesystem::exists(curr)) {
            cerr << "The file " << curr.string() << " does not exist!" << endl;
            cerr << "Please supply a valid file name." << endl;
            return;
        }
        objectFiles.push_back(curr);
    }
    if (!suppress) {
        //Next, reads the directories.
        vector<path> dirFiles = TAFunctions::getObjectFiles(graph, curPath, path());
        objectFiles.insert(objectFiles.end(), dirFiles.begin(), dirFiles.end());
    }

    if (objectFiles.size() == 0 && !suppress){
        cerr << "No object files found in " << curPath.string() << " directory!" << endl
             << "The program will now exit without performing analysis." << endl;
        return;
    } else if (objectFiles.size() == 0 && suppress){
        cerr << "No object files supplied to the program!" << endl
             << "The program will now exit without performing analysis." << endl;
        return;
    }
    cout << endl;

    //Perform the removals.
    for (string removal : removeFiles){
        cout << "Removing " << removal << "...";
        path p = path(removal);

        for (int i = 0; i < objectFiles.size(); i++){
            path curFile = objectFiles.at(i);

            if (p.string().compare(curFile.string()) == 0){
                objectFiles.erase(objectFiles.begin() + i);
                cout << "removed!" << endl;
                break;
            }

            if (i + 1 == objectFiles.size()){
                cout << "not found!" << endl;
            }
        }
    }

    //Finally check if we have a valid list.
    if (objectFiles.size() == 0){
        cerr << "No object files found in the queue to be processed!" << endl
             << "The program will now exit without performing analysis." << endl;;
        return;
    }

    cout << endl;

    //Once found, we process each individually.
    for (int i = 0; i < objectFiles.size(); i++){
        cout << "Processing " << objectFiles.at(i).string() << "...\n";
        process(objectFiles.at(i));
    }
    cout << endl;

    //Now, we resolve all undefined references.
    cout << "Resolving all undefined references..." << endl;
    processUndefinedReferences();
    cout << endl;

    //Next, we generate the TA file.
    TAFunctions::generateTAFile(outputDirectory, graph);
}

/**
 * Processes a singular object file by
 * inspecting the symbol table and then
 * examining the relocations in the file.
 * @param objectFile The object file being examined.
 */
void ElfReader::process(path objectFile){
    //Start by reading the object file.
    elfio reader;
    if (!reader.load(objectFile.string())){
        cout << "\t- Could not read object file. Ignoring..." << "\n";
        return;
    }

    //Next, print the file properties.
    string properties = "\t- Reading a ";
    if (reader.get_class() == ELFCLASS32){
        properties += "32bit";
    } else {
        properties += "64bit";
    }
    properties += " object file that is ";
    if (reader.get_encoding() == ELFDATA2LSB){
        properties += "little endian.";
    } else {
        properties += "big endian.";
    }
    cout << properties << "\n";

    //We find the symbol table in our file.
    Elf_Half sec_num = reader.sections.size();
    for (int i = 0; i < sec_num; i++){
        section* currSec = reader.sections[i];

        //Check if we have the symbol table.
        if (currSec->get_type() == SHT_SYMTAB){
            //Process the symbol table initially.
            cout << "\t- Performing initial pass." << endl;
            processSymbolTable(objectFile, currSec);

            //Link references.
            cout << "\t- Resolving and linking references." << endl;
            resolveReferences(objectFile, currSec);
        }
    }
}

/**
 * Helper method that examines the symbol table and generates nodes
 * for the graph.
 * @param oFile Object file being examined.
 * @param symTab Symbol table section pointer.
 */
void ElfReader::processSymbolTable(path oFile, section* symTab){
    //Create variables for symbol table information.
    string name;
    Elf64_Addr value;
    Elf_Xword size;
    unsigned char bind;
    unsigned char type;
    Elf_Half section_index;
    unsigned char other;

    //Generate a symbol table accessor.
    elfio reader = elfio();
    reader.load(oFile.string());
    const symbol_section_accessor symbols(reader, symTab);

    //Next, inspect each entry in the table.
    for (unsigned int i = 0; i < symbols.get_symbols_num(); i++){
        //Read the associated symbol.
        symbols.get_symbol(i, name, value, size, bind, type, section_index, other);

        //Check if we have an undefined reference.
        if (section_index == SHN_UNDEF){
            //It is likely defined in another file.
            continue;
        }

        //Next, check what type of symbol we're dealing with.
        if (type == STT_FUNC || type == STT_OBJECT) {
            //Generate a UNIQUE ID for the symtab object.
            string ID = generateID(oFile.string(), section_index, value);
            string demName = demangleName(name.c_str());
            string fileName = oFile.filename().string();

            //Add entry into our graph. Ensure that we have
            graph->addNode(ID, (type == STT_FUNC) ? BFXNode::FUNCTION : BFXNode::OBJECT, demName, name);
            if (!graph->doesContainEdgeExist(fileName, ID)){
                bool success = graph->addEdge(fileName, ID, BFXEdge::CONTAINS);
                if (!success) {
                    cerr << "Error adding a function/object to file! File doesn't exist!" << endl
                            << "Program will now exit." << endl;
                    _exit(1);
                }
            }
        }
    }
}

/**
 * Helper method that resolves references from the symbol table.
 * Only works if the symbols have all being examined first.
 * @param oFile Object file being examined.
 * @param symTab Symbol table section pointer.
 */
void ElfReader::resolveReferences(path oFile, section* symTable){
    //Create variables for symbol table information.
    string name;
    Elf64_Addr value;
    Elf_Xword size;
    unsigned char bind;
    unsigned char type;
    Elf_Half section_index;
    unsigned char other;

    //Generate a symbol table accessor.
    elfio reader = elfio();
    reader.load(oFile.string());
    const symbol_section_accessor symbols(reader, symTable);

    //Reiterate and inspect each entry.
    for (unsigned int i = 0; i < symbols.get_symbols_num(); i++){
        //Read the associated symbol.
        symbols.get_symbol(i, name, value, size, bind, type, section_index, other);
        if (type != STT_FUNC && type != STT_OBJECT) continue;

        //Find the relocation entry.
        int relocation_num = getRelocationSection(oFile.string(), section_index);
        if (relocation_num == -1) continue;

        //Generate a relocation accessor.
        elfio relReader = elfio();
        relReader.load(oFile.string());
        section* currSection = relReader.sections[relocation_num];
        const relocation_section_accessor relInfo(relReader, currSection);

        //Variables for relocation entry.
        Elf64_Addr offset;
        Elf64_Addr symbolValue;
        string symbolName;
        Elf_Word type;
        Elf_Sxword addend;
        Elf_Sxword calcValue;

        //Now process the relocations.
        for (unsigned int i = 0; i < relInfo.get_entries_num(); i++) {
            //Get the value.
            relInfo.get_entry(i, offset, symbolValue, symbolName, type, addend, calcValue);

            //Check the address.
            if (isValidReloc(value, value + size, offset)){
                if (symbolName.compare("") == 0 ||
                        graph->doesMangleEdgeExist(name, symbolName)) continue;

                //Add an edge from that node to the ID of the other node.
                bool result = graph->addEdgeByMangle(name, symbolName, BFXEdge::LINK);

                //Check if the edge was added.
                if (!result){
                    //We add it to our external reference graph.
                    if (externalRef.find(name) == externalRef.end()){
                        //Create a new vector and add it.
                        externalRef[name] = vector<string>();
                    }

                    //Add it to our reference list.
                    externalRef[name].push_back(symbolName);
                }
            }
        }
    }
}

/**
 * Examines all references that were not found
 * during the first pass through the O files.
 * The process function needs to be used on all
 * object files before this function can be used.
 */
void ElfReader::processUndefinedReferences(){
    map<string, vector<string>>::iterator it;

    //We iterate through the map.
    for (it = externalRef.begin(); it != externalRef.end(); it++){
        //Get the ID.
        string srcMangle = it->first;
        vector<string> dstMangles = it->second;

        //Iterate through our vector to add all references.
        for (int i = 0; i < dstMangles.size(); i++){
            string dstMangle = dstMangles.at(i);

            //Add the edge.
            graph->addEdgeByMangle(srcMangle, dstMangle, BFXEdge::LINK);
        }
    }
}

/**
 * Generates an ID for the object. This is based on the file it is in
 * and the location in the O file. Different than the mangled name.
 * @param path The O file location.
 * @param sectionNum The section number that the symbol exists.
 * @param addr The address that it starts at in that section.
 * @return A string ID generated.
 */
string ElfReader::generateID(string path, Elf_Half sectionNum, Elf64_Addr addr){
    string ID = path + "[";

    //Generate the reader.
    elfio reader = elfio();
    reader.load(path);

    //Next, get the associated section.
    section* secVal = reader.sections[sectionNum];
    ID += secVal->get_name() + "+0x";

    //Get the hex value of the address.
    stringstream hexStream;
    hexStream << hex << addr;
    ID += hexStream.str() + "]";

    return ID;
}

/**
 * Demangles a mangled symbol. If it cannot be demangled,
 * this function passed the mangled name back.
 * @param mangledName The symbol mangled.
 * @return The demangled version.
 */
string ElfReader::demangleName(const char* mangledName){
    int status;

    //Gets the unmangled name.
    char *ret = abi::__cxa_demangle(mangledName, 0, 0, &status);

    //Resolves the status.
    if (status != 0){
        //Pass back the mangled name.
        return string(mangledName);
    }

    return string(ret);
}

/**
 * Gets the relocation section for an associated section.
 * If the section cannot be found, returns -1.
 * @param path The O file.
 * @param secNum The section num.
 * @return A section num indicating where the relocation section is.
 */
int ElfReader::getRelocationSection(string path, Elf_Half secNum){
    elfio reader = elfio();
    reader.load(path);

    //Get the name of the section.
    string secName = reader.sections[secNum]->get_name();

    //Next, find the relocation section.
    Elf_Half secSize = reader.sections.size();
    for (int i = 0; i < secSize; i++){
        section* current = reader.sections[i];
        string name = current->get_name();

        //Iterate through the relocation prefixes.
        for (string curPrefix : REL_PREFIX){
            if (name.compare(curPrefix + secName) == 0)
                return i;
        }
    }

    return -1;
}

/**
 * Checks whether the relocation is being applied to the current object.
 * @param startPos The start address of the object.
 * @param endPos The end address of the object.
 * @param relocPos The relocation address.
 * @return Whether the relocation address falls between start and end.
 */
bool ElfReader::isValidReloc(Elf64_Addr startPos, Elf64_Addr endPos, Elf64_Addr relocPos){
    //Check the relocation entry position against the current symbol.
    if (relocPos < startPos) return false;
    else if (relocPos >= startPos && relocPos >= endPos) return false;

    return true;
}
