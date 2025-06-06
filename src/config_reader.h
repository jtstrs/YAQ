#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <unordered_map>

class ConfigReader {
public:
    virtual std::unordered_map<std::string, std::string> read(std::istream& src) = 0;
};

#endif // CONFIG_H
