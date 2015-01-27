#ifndef RAWFILE_H
#define RAWFILE_H

#include <unistd.h>
#include <string>

class RawFile {
public:
    RawFile();
    virtual ~RawFile();

    virtual bool Open(std::string fname);
    virtual bool Close();
    virtual int Read(void* buf, size_t count);
    virtual int Write(const void* buf, size_t count);
    virtual std::string ReadLine();
    virtual bool Append(std::string value);
    virtual bool Truncate();
    virtual void LSeek(off_t offset);
};

#endif
