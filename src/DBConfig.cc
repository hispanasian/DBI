#include "../include/DBConfig.h"
#include "../include/RawFile.h"
#include <iostream>

using namespace std;

DBConfig::DBConfig() {}

DBConfig::~DBConfig() {}

bool DBConfig::Read(RawFile &file) {
    file.Truncate();
    return false;
}

bool DBConfig::Write(RawFile &file) {
    return false;
}

void DBConfig::AddKey(string key, string value) {
}

std::string DBConfig::GetKey(string key) {
    return "";
}