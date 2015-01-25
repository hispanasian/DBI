#include "../include/DBConfig.h"
#include "../include/RawFile.h"
#include <iostream>

using namespace std;

DBConfig::DBConfig() {}

DBConfig::~DBConfig() {}

bool DBConfig::Read(RawFile &file) {
    // Let's assume that file is at the beginning
    // TODO: Update to deal with lseek so this assumption isn't made.
    bool success = true;
    std::string line;
    std::string empty = "";
    std::string key;
    std::string val;
    int delim;

    while(success && empty.compare(line = file.ReadLine()) != 0) {
        delim = line.find_first_of("=");
        if(delim == string::npos) success = false;
        else {
            key = line.substr(0, delim);
            val = line.substr(delim + 1, line.length() + 1);
            map.insert(std::pair<std::string, std::string>(key, val));
        }
    }
    success = file.Close() && success;
    return success;
}

bool DBConfig::Write(RawFile &file) {
    return false;
}

void DBConfig::AddKey(string key, string value) {
}

std::string DBConfig::GetKey(string key) {
    return "";
}