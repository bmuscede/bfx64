//
// Created by bmuscede on 12/02/17.
//

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
