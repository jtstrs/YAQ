#include "logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>

Logger::Logger()
    : log_level_(LogLevel::INFO)
{
}

void Logger::setLogLevel(LogLevel level)
{
    log_level_ = level;
}

LogLevel Logger::getLogLevel() const
{
    return log_level_;
}

void Logger::debug(const std::string& message)
{
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message)
{
    log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message)
{
    log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message)
{
    log(LogLevel::ERROR, message);
}

void Logger::log(LogLevel level, const std::string& message)
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    if (level >= log_level_) {
        std::cout << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] " << "[" << levelToString(level) << "] " << message << std::endl;
    }
}

std::string Logger::levelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}
