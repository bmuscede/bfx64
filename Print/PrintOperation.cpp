/////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrintOperation.h
//
// Created By: Bryan J Muscedere
// Date: 12/02/17
//
// Performs operations for printing.
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
#include <ncurses.h>
#include "PrintOperation.h"

using namespace std;

/**
 * Sets up a printer to print status.
 * @param verbose Whether we're in verbose mode.
 * @param numFiles The number of files to process.
 */
PrintOperation::PrintOperation(bool verbose, int numFiles){
    this->verbose = verbose;

    this->numFiles = numFiles;
    this->curFile = 0;
    this->printed = false;

    bar = nullptr;
}

/**
 * Destructor
 */
PrintOperation::~PrintOperation() { }

/**
 * Sets the number of files.
 * @param numFiles The number of files to process.
 */
void PrintOperation::setNumFiles(int numFiles){
    this->numFiles = numFiles;
}

/**
 * Gets the number of files.
 * @return The number of files.
 */
int PrintOperation::getNumFiles(){
    return numFiles;
}

/**
 * Gets the current file number.
 * @return
 */
int PrintOperation::getCurFileNum(){
    return curFile;
}

/**
 * Print that a file was found.
 * @param fileName The file that was found.
 */
void PrintOperation::printFileFound(string fileName) {
    if (!verbose) return;

    cout << "Found: " << fileName << endl;
}

/**
 * Prints any desired line.
 * @param line The line to print.
 */
void PrintOperation::printLine(std::string line){
    cout << line << endl;
}

/**
 * Notifies that file search is started.
 */
void PrintOperation::printStartFileSearch() {
    if (!verbose) cout << "Searching for object files..." << flush;
}
void PrintOperation::printFileProcess(string fileName) {
    if (!verbose) {
        printFileProcessNVerbose();
        return;
    }

    string outputMessage = "";

    //Check if we have a num files limit.
    if (numFiles != 0){
        //Ups the current file.
        curFile++;
        outputMessage += "(" + to_string(curFile) + " / " + to_string(numFiles) + ") ";
    }

    //Prints the rest of the message.
    outputMessage += "Processing " + fileName + "..." + "\n";
    cout << outputMessage;
}

/**
 * Prints the current file info.
 * @param bitType The bit number.
 * @param endianness The endianness.
 */
void PrintOperation::printFileProcessSub(PrintOperation::Bit bitType, PrintOperation::Endian endianness) {
    if (!verbose) {
        printFileProcessNVerbose();
        return;
    }

    string msg = "\t- Reading a ";

    //Gets the bit type.
    switch (bitType){
        case x86:
            msg += "32bit";
            break;
        case x64:
            msg += "64bit";
    }
    msg += " object file that is ";

    //Gets the endianness.
    switch (endianness){
        case LITTLE:
            msg += "little endian.";
            break;
        case BIG:
            msg += "big endian.";
    }

    cout << msg << endl;
}

/**
 * Prints the current progress.
 * @param opType The current operation.
 */
void PrintOperation::printFileProcessSub(PrintOperation::Operation opType) {
    if (!verbose && (opType != INVALID || opType != PURGE)) {
        printFileProcessNVerbose();
        return;
    } else if (!verbose && opType == INVALID){
        printFileProcessNVerbose();
        printFileProcessNVerbose();
        printFileProcessNVerbose();
    }

    switch (opType){
        case INVALID:
            cout << "\t- Could not read object file. Ignoring..." << endl;
            break;
        case INITIAL:
            cout << "\t- Performing initial pass." << endl;
            break;
        case LINK:
            cout << "\t- Resolving and linking references." << endl;
            break;
        case PURGE:
            cout << "Purging graph contents to TA file..." << endl;
    }
}

/**
 * Prints that files are being resolved.
 */
void PrintOperation::printResolving() {
    cout << "Resolving all undefined references...";
}

/**
 * Prints that files are done being resolved.
 */
void PrintOperation::printDoneResolving() {
    cout << "done!" << endl << endl;
}

/**
 * Print that file isn't found
 * @param fileName The file not found.
 */
void PrintOperation::printFileNotFound(string fileName) {
    cerr << "The file " << fileName << " does not exist!" << endl;
    cerr << "Please supply a valid file name." << endl;
}

/**
 * Prints that no files were found.
 */
void PrintOperation::printNoFiles() {
    cerr << "No object files supplied to the program!" << endl
         << "The program will now exit without performing analysis." << endl;
}

/**
 * Prints that files are done being searched.
 */
void PrintOperation::printDoneFileSearch(){
    //Check what we do.
    if (verbose) {
        cout << endl;
        return;
    }

    //If not verbose, we do this.
    cout << "done!" << endl << endl;
}

/**
 * Prints that the process is starting.
 */
void PrintOperation::printStartProcess(){
    //Do nothing for verbose.
    if (verbose) return;

    //Create the progress bar.
    if (numFiles == 0){
        cout << "Processing files..." << endl;
    } else {
        bar = progressbar_new("Processing", (unsigned int) numFiles * 3);
    }
}

/**
 * Prints the end of the process.
 */
void PrintOperation::printEndProcess(){
    //If verbose, print a new line.
    if (verbose) {
        cout << endl;
        return;
    }

    //If not verbose, terminate the progress bar.
    if (bar != nullptr) progressbar_finish(bar);
    cout << endl;
}

/**
 * Prints success message.
 * @param fileName File being outputted.
 */
void PrintOperation::printTASuccess(std::string fileName){
    cout << "TA file successfully written to " << fileName << "!" << endl;
}

/**
 * Prints failure message.
 * @param fileName File being outputted.
 */
void PrintOperation::printTAFailure(std::string fileName){
    cout << "TA file could not be written to " << fileName << "!" << endl;
    cout << "Check appropriate file permissions." << endl;
}

/**
 * Prints the current file process.
 */
void PrintOperation::printFileProcessNVerbose() {
    //Check the progress bar.
    if (bar == nullptr) return;

    //Increment it.
    progressbar_inc(bar);
}