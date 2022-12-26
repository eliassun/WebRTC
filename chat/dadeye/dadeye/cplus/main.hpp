//
//  main.h
//  dadeye
//
//  Created by Elias Sun on 12/24/22.
//


#include <string>

class DadEyeWrapper {
public:
    void dadmode(const char* sweetid);
    void sweetmode(const char* dadid, const char* path);
    void generateid();
    void getscreen(const char* path);
    void display(const char *path);
};

const uint64_t codeBaseID   =  1000;
const uint64_t codeDadID    =  codeBaseID + 1;
const uint64_t codeSweetID  =  codeBaseID + 2;
const uint64_t codeExit     =  codeBaseID + 3;
const uint64_t codeDebug    =  codeBaseID + 4;
const uint64_t codeInfo     =  codeBaseID + 5;
const uint64_t codeWarning  =  codeBaseID + 6;
const uint64_t codeError    =  codeBaseID + 7;
const uint64_t codeCritical =  codeBaseID + 8;
const uint64_t codeMyID     =  codeBaseID + 9;
const uint64_t codeDadReady =  codeBaseID + 10;
const uint64_t codeSweetReady = codeBaseID + 11;
