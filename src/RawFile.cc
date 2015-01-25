#include "../include/RawFile.h"

bool RawFile::Open(std::string fname) {
    return false;
}

bool RawFile::Close() {
    return false;
}

bool RawFile::Append(std::string value) {
    return false;
}

int RawFile::Read(void *buf, size_t count) {
    return 0;
}

std::string RawFile::ReadLine() {
    return "";
}

bool RawFile::Truncate() {
    return false;
}

int RawFile::Write(const void *buf, size_t count) {
    return 0;
}

