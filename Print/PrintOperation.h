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

#ifndef BFX64_PRINTOPERATION_H
#define BFX64_PRINTOPERATION_H

#include <string>
#include "ProgressBar.h"

class PrintOperation {
public:
    enum Bit {x64, x86};
    enum Endian {LITTLE, BIG};
    enum Operation {INVALID, INITIAL, LINK, PURGE};

    PrintOperation(bool verbose, int numFiles = 0);
    ~PrintOperation();

    void setNumFiles(int numFiles);

    int getNumFiles();
    int getCurFileNum();

    void printLine(std::string line);
    void printStartFileSearch();
    void printFileFound(std::string fileName);
    void printFileProcess(std::string fileName);
    void printFileProcessSub(PrintOperation::Bit bitType, PrintOperation::Endian endianness);
    void printFileProcessSub(PrintOperation::Operation opType);

    void printResolving();
    void printDoneResolving();

    void printFileNotFound(std::string fileName);
    void printNoFiles();

    void printDoneFileSearch();
    void printStartProcess();
    void printEndProcess();

    void printTASuccess(std::string fileName);
    void printTAFailure(std::string fileName);

private:
    bool verbose;
    int curFile;
    int numFiles;
    bool printed = false;
    progressbar* bar;

    void printFileProcessNVerbose();

};


#endif //BFX64_PRINTOPERATION_H
