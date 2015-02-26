#include "../include/DBConfig.h"
#include "../include/RawFile.h"
#include <iostream>

using namespace std;

DBConfig::DBConfig() {}

DBConfig::~DBConfig() {}

bool DBConfig::Read(RawFile &file) {
    bool success = true;
    std::string* line = new std::string();
    std::string key;
    std::string val;
    int delim;

    file.LSeek(0); // Go to the beginning of the file
    while(success && file.ReadLine(line)) {
        delim = line->find_first_of("=");
        if(delim == string::npos) success = false;
        else {
        	cout << "DBConfig:: Read is reading " + line->substr(0, line->length() + 1) << endl;
            key = line->substr(0, delim);
            cout << "key = " << key << endl;
            val = line->substr(delim + 1, line->length() + 1);
            cout << "val = " << val << endl;
            map.insert(pair<string, string>(key, val));
        }
    }
    delete line;
    return success;
}

bool DBConfig::Write(RawFile &file) {
    bool success = file.Truncate();
    if(success) file.LSeek(0); // Go to the beginning of the file

    for(std::map<string, string>::iterator it = map.begin(); success && it != map.end(); it++) {
        success = file.Append(it->first + "=" + it->second + "\n");
    }
    return success;
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

void DBConfig::ReplaceKey(string key, string value) {
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
	map.erase(key);
	map.insert(pair<string, string>(key, value));
}

void DBConfig::Clear() {
	map.clear();
}
