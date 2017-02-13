//
// Created by bmuscede on 12/02/17.
//

#include <iostream>
#include <ncurses.h>
#include "PrintOperation.h"

using namespace std;

PrintOperation::PrintOperation(bool verbose, int numFiles){
    this->verbose = verbose;

    this->numFiles = numFiles;
    this->curFile = 0;
    this->printed = false;

    bar = nullptr;
}

PrintOperation::~PrintOperation() { }

void PrintOperation::setNumFiles(int numFiles){
    this->numFiles = numFiles;
}

int PrintOperation::getNumFiles(){
    return numFiles;
}

int PrintOperation::getCurFileNum(){
    return curFile;
}

void PrintOperation::printFileFound(string fileName) {
    if (!verbose) return;

    cout << "Found: " << fileName << endl;
}

void PrintOperation::printLine(std::string line){
    cout << line << endl;
}

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

void PrintOperation::printFileProcessSub(PrintOperation::Operation opType) {
    if (!verbose) {
        printFileProcessNVerbose();
        return;
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
    }
}

void PrintOperation::printResolving() {
    cout << "Resolving all undefined references...";
}

void PrintOperation::printDoneResolving() {
    cout << "done!" << endl << endl;
}

void PrintOperation::printFileNotFound(string fileName) {
    cerr << "The file " << fileName << " does not exist!" << endl;
    cerr << "Please supply a valid file name." << endl;
}

void PrintOperation::printNoFiles() {
    cerr << "No object files supplied to the program!" << endl
         << "The program will now exit without performing analysis." << endl;
}

void PrintOperation::printDoneFileSearch(){
    //Check what we do.
    if (verbose) {
        cout << endl;
        return;
    }

    //If not verbose, we do this.
    cout << "done!" << endl << endl;
}

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

void PrintOperation::printTASuccess(std::string fileName){
    cout << "TA file successfully written to " << fileName << "!" << endl;
}

void PrintOperation::printTAFailure(std::string fileName){
    cout << "TA file could not be written to " << fileName << "!" << endl;
    cout << "Check appropriate file permissions." << endl;
}

void PrintOperation::printFileProcessNVerbose() {
    //Check the progress bar.
    if (bar == nullptr) return;

    //Increment it.
    progressbar_inc(bar);
}