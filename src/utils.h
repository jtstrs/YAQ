#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cctype>
#include <string>

namespace utils {

// Trim whitespace from the left side of a string
inline std::string ltrim(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// Trim whitespace from the right side of a string
inline std::string rtrim(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(),
        s.end());
    return s;
}

// Trim whitespace from both sides of a string
inline std::string trim(std::string s)
{
    return ltrim(rtrim(std::move(s)));
}

} // namespace utils

#endif // UTILS_H