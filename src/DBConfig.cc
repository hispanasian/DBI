#include "../include/DBConfig.h"
#include "../include/RawFile.h"
#include <iostream>

using namespace std;

DBConfig::DBConfig() {}

DBConfig::~DBConfig() {}

bool DBConfig::Read(RawFile &file) {
    bool success = true;
    std::string line;
    std::string empty = "";
    std::string key;
    std::string val;
    int delim;

    file.LSeek(0); // Go to the beginning of the file
    while(success && empty.compare(line = file.ReadLine()) != 0) {
        delim = line.find_first_of("=");
        if(delim == string::npos) success = false;
        else {
            key = line.substr(0, delim);
            val = line.substr(delim + 1, line.length() + 1);
            map.insert(pair<string, string>(key, val));
        }
    }
    success = file.Close() && success;
    return success;
}

bool DBConfig::Write(RawFile &file) {
    bool success = file.Truncate();
    if(success) file.LSeek(0); // Go to the beginning of the file

    for(std::map<string, string>::iterator it = map.begin(); success && it != map.end(); it++) {
        success = file.Append(it->first + "=" + it->second + "\n");
    }

    success = file.Close() && success;
    return success;
    file.Truncate();
    // Let's assume that file is at the beginning

    return false;
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
    // Validate key. key cannot contain '\r', '\n' or '='
    if(key.find("=") != string::npos ||
            key.find("\r") != string::npos ||
            key.find("\n") != string::npos) {
        throw IllegalKeyException();
    }

    // Validate value. value cannot contain '\r' or '\n'
    if(value.find("\r") != string::npos || value.find("\n") != string::npos) {
        throw IllegalValueException();
    }
    map.insert(pair<string, string>(key, value));
}

std::string DBConfig::GetKey(string key) {
    string value;
    try {
        value = map.at(key);
    } catch(out_of_range e) {
        value = "";
    }
    return value;
}
    try {
        value = map.at(key);
    } catch(out_of_range e) {
        value = "";
    }
    return value;
}

bool DBConfig::Write(RawFile &file) {
    return false;
}

void DBConfig::AddKey(string key, string value) {
}

std::string DBConfig::GetKey(string key) {
    return "";
}
