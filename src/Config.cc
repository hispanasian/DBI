#include "../include/Config.h"
#include "../include/RawFile.h"
#include <iostream>

using namespace std;

Config::Config() {
    changed = false;
}

Config::Config(RawFile file) {
    this->file = file;
    Config();
}

Config::~Config() {}

bool Config::Open(std::string fname) {
    file.Truncate();
    return file.Open(fname);
}

bool Config::Close() {
    return false;
}

void Config::AddKey(std::string key, string value) {
}

std::string Config::GetKey(std::string key) {
    return "";
}