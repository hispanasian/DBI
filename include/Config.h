#ifndef CONFIG_H
#define CONFIG_H

#include <iosfwd>
#include <map>
#include "RawFile.h"

class Config {
private:
    RawFile file;
    std::map<std::string, std::string> map;
    bool changed;

public:
    Config();
    Config(RawFile file);
    virtual ~Config();

    /**
    * Open the config file located at fname.
    * @param fname  The name of the file holding the config
    * @return       True if the file opened successfully.
    */
    virtual bool Open(std::string fname);

    /**
    * If there has been a change (a new key added), over-write the RawFile used by this config
    * and then close the RawFile.
    * @return   True if the file closed without issue.
    */
    virtual bool Close();

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

#endif CONFIG_H