#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();
    void setLogLevel(LogLevel level);
    LogLevel getLogLevel() const;
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    LogLevel log_level_;
    void log(LogLevel level, const std::string& message);
    std::string levelToString(LogLevel level);
};

#endif // LOGGER_H
