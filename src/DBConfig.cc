#include "../include/DBConfig.h"
#include "../include/RawFile.h"
#include <iostream>

using namespace std;

DBConfig::DBConfig() {}

DBConfig::~DBConfig() {}

bool DBConfig::Read(RawFile &file) {
    bool success = true;
    return success;
}

bool DBConfig::Write(RawFile &file) {
    bool success = file.Truncate();
    return success;
}

void DBConfig::AddKey(string key, string value) {
}

std::string DBConfig::GetKey(string key) {
    string value;
    return value;
}

void DBConfig::ReplaceKey(string key, string value) {
}

void DBConfig::Clear() {

}

bool DBConfig::Write(RawFile &file) {
    return false;
}

void DBConfig::AddKey(string key, string value) {
}

std::string DBConfig::GetKey(string key) {
    return "";
}
