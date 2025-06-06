#ifndef INI_CONFIG_READER_H
#define INI_CONFIG_READER_H

#include "config_reader.h"

class IniConfigReader : public ConfigReader {
public:
    std::unordered_map<std::string, std::string> read(std::istream& src) override;
};

#endif // DEFAULT_CONFIG_READER_H
