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
public:
    RawFile();
    virtual ~RawFile();

    virtual bool Open(std::string fname);
    virtual bool Open(FILE *_file);
    virtual bool Close();
    virtual int Read(void* buf, size_t count);
    virtual int Write(const void* buf, size_t count);
    virtual bool ReadLine(std::string* line);
    virtual bool Append(std::string value);
    virtual bool Truncate();
    virtual void LSeek(off_t offset);
    virtual bool FileExists(const char* fileName);
    virtual int Rename(const char *oldName, const char *newName);
    virtual int Remove(const char *name);

    /*
     * Takes in a char array scheme with the following pattern: filenameXXXXXX where the X's will
     * be replaced accordingly and used to generate and create a unique file. Note that char* is
     * not constant because it WILL be changed. Note that this must end with 6 trailing X's
     */
    virtual void MakeTemp(char *scheme);
};

#endif RAWFILE_H
