#include <sstream>
#include "RawFile.h"

RawFile::RawFile() {
}

RawFile::~RawFile() {
	if(file != NULL) {
		fclose(file);
	}
}

bool RawFile::Open(std::string fname) {
	filename = fname;
	std::string mode("r+");
	if(!FileExists(fname.c_str())) {
		mode = "w+";
	}
	file = fopen(fname.c_str(), mode.c_str());
	return file != NULL;
}

bool RawFile::Close() {
	int ret = fclose(file);
	file = NULL;
	return ret == 0;
}

int RawFile::Read(void* buf, size_t count) {
	return fread(buf, 1, count, file);
}

int RawFile::Write(const void* buf, size_t count) {
	return fwrite(buf, 1, count, file);
}

bool RawFile::ReadLine(std::string* line) {
	std::stringstream ss;
	bool newLineFound = false;
	// append chars until a newline is found
	while(!newLineFound) {
		int bytesRead = fread(buffer, 1, sizeof(buffer), file);
		if(bytesRead == 0) {
			// we've reached the end of the file
			break;
			// return false;
		}
		for(int i = 0; i < bytesRead; ++i) {
			if(buffer[i] == '\n' || buffer[i] == '\r') {
				// move the file cursor to the byte past this newline
				fseek(file, i-bytesRead+1, SEEK_CUR);
				newLineFound = true;
				break;
			} else {
				// not a newline, add the character to the output
				ss << buffer[i];
			}
		}
	}
	line->clear();
	for(int i = 0; i < ss.str().size(); ++i) {
		line->push_back(ss.str()[i]);
	}
	return ss.str().size() > 0;
}

bool RawFile::Append(std::string value) {
	long pos = ftell(file);
	fseek(file, 0, SEEK_END);
	int bytesWritten = fwrite(value.c_str(), 1, value.size(), file);
	fseek(file, pos, SEEK_CUR);
	return bytesWritten == value.size();
}

bool RawFile::Truncate() {
	int ret = fclose(file);
	if(ret != 0) return false;
	file = fopen(filename.c_str(), "w+");
	return file != NULL;
}

void RawFile::LSeek(off_t offset) {
	fseek(file, offset, SEEK_SET);
}

bool RawFile::FileExists(const char* fileName) {
	return access(fileName, F_OK) != -1;
}

void RawFile::MakeTemp(char *scheme) {
	int fd = mkstemp(scheme);
	close(fd);
}

int RawFile::Rename(char *oldName, char *newName) {
	return rename(oldName, newName);
}
