/*
   capture.cpp and capture.h
   Copyright (C) 2022-2028 Elias Sun
*/

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "base64.h"

using namespace std;

string encode_image(const string& path) {
    vector<char> temp;
    std::ifstream infile;
    infile.open(path, ios::binary);
    if (infile.is_open()) {
        while (!infile.eof()) {
            char c = (char)infile.get();
            temp.push_back(c);
        }
        infile.close();
    }
    else return "File could not be opened";
    string ret(temp.begin(), temp.end() - 1);
    ret = base64_encode((unsigned const char*)ret.c_str(), ret.size());
    return ret;
}

void decode_image(const string& input) {
    ofstream outfile;
    outfile.open("./test_dec.png", ofstream::binary);
    string temp = base64_decode(input);
    outfile.write(temp.c_str(), temp.size());
    outfile.close();
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

string capture_screen() {
   string path = "/usr/sbin/screencapture";
   std::string result = exec("/usr/sbin/screencapture -C -x /tmp/picap.png");
   return encode_image("/tmp/picap.png");
}

void open_image() {
    exec("open ./test_dec.png");
}
