#ifndef RAWFILE_H
#define RAWFILE_H

#include <unistd.h>
#include <stdio.h>
#include <string>

class RawFile {
private:
	FILE* file = NULL;
	char buffer[4096];
	std::string filename;
	bool fileExists(const char* fileName);
public:
    RawFile();
    virtual ~RawFile();

    virtual bool Open(std::string fname);
    virtual bool Close();
    virtual int Read(void* buf, size_t count);
    virtual int Write(const void* buf, size_t count);
    virtual bool ReadLine(std::string* line);
    virtual bool Append(std::string value);
    virtual bool Truncate();
    virtual void LSeek(off_t offset);
};

#endif RAWFILE_H
