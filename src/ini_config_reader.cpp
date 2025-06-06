#include "ini_config_reader.h"
#include <boost/format.hpp>
#include <stdexcept>

std::unordered_map<std::string, std::string> IniConfigReader::read(std::istream& src)
{
    std::unordered_map<std::string, std::string> config;

    std::string line;
    for (std::size_t line_index = 0; std::getline(src, line); line_index++) {
        std::string key;
        std::string value;

        std::size_t pos = line.find('=');

        if (pos == std::string::npos) {
            throw std::runtime_error((boost::format("Bad config line %d") % line_index).str());
        }

        key = line.substr(0, pos);
        value = line.substr(pos + 1);

        if (key.empty() || value.empty()) {
            throw std::runtime_error((boost::format("Bad config line %d") % line_index).str());
        }
        config[std::move(key)] = std::move(value);
    }
    return config;
}
