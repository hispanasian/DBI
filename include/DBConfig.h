#ifndef DBCONFIG_H
#define DBCONFIG_H

#include <iosfwd>
#include <map>
#include "RawFile.h"

class DBConfig {
friend class DBConfigTest;
private:
    std::map<std::string, std::string> map;

public:
    DBConfig();
    virtual ~DBConfig();

    /**
    * Takes open file, builds a map based on the contents, and closes it. This function assumes
    * that file is at the beginning. DBConfig expects every line to be of the format: "key=value".
    * If there is an invalid line, Read will return false. Any data obtained during Read should be
    * considered not-safe to use. If Read encounters multiple keys with the same value, Read will
    * keep the first key in the file.
    * @param file   The RawFile with config information
    * @return       True if the file was opened, read, and closed successfully.
    */
    virtual bool Read(RawFile &file);

    /**
    * Writes the internal map to file.
    * @return   True if the file opened, was written to, and closed successfully.
    */
    virtual bool Write(RawFile &file);

    /**
    * Add a key-value to the Config.
    * @param key    The key associated with value.
    * @param value  The value being mapped to by key.
    */
    virtual void AddKey(std::string key, std::string value);

    /**
    * Returns the value associated value to key or an empty string if no key was found.
    * @param key    The key associated with the returning value.
    * @return       The value associated with key or an empty string if no key was found.
    */
    virtual std::string GetKey(std::string key);
};

#endif DBCONFIG_H