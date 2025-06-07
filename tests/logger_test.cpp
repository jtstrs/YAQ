#include "../src/logger.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

// Helper class to capture cout output
class CoutCapture {
public:
    CoutCapture()
        : old_cout(std::cout.rdbuf())
    {
        std::cout.rdbuf(buffer.rdbuf());
    }

    ~CoutCapture()
    {
        std::cout.rdbuf(old_cout);
    }

    std::string getOutput()
    {
        return buffer.str();
    }

private:
    std::stringstream buffer;
    std::streambuf* old_cout;
};

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        // Reset logger to default state before each test
        logger = &Logger::getInstance();
        logger->setLogLevel(LogLevel::INFO);
    }

    Logger* logger;
};

TEST_F(LoggerTest, SingletonInstance)
{
    Logger& instance1 = Logger::getInstance();
    Logger& instance2 = Logger::getInstance();
    EXPECT_EQ(&instance1, &instance2) << "getInstance() should return the same instance";
}

TEST_F(LoggerTest, LogLevelSetting)
{
    logger->setLogLevel(LogLevel::DEBUG);
    EXPECT_EQ(LogLevel::DEBUG, logger->getLogLevel());

    logger->setLogLevel(LogLevel::ERROR);
    EXPECT_EQ(LogLevel::ERROR, logger->getLogLevel());
}

TEST_F(LoggerTest, LogLevelFiltering)
{
    CoutCapture capture;

    // Set to ERROR level
    logger->setLogLevel(LogLevel::ERROR);

    logger->debug("Debug message");
    logger->info("Info message");
    logger->warn("Warning message");
    logger->error("Error message");

    std::string output = capture.getOutput();
    EXPECT_TRUE(output.find("Debug message") == std::string::npos);
    EXPECT_TRUE(output.find("Info message") == std::string::npos);
    EXPECT_TRUE(output.find("Warning message") == std::string::npos);
    EXPECT_TRUE(output.find("Error message") != std::string::npos);
}

TEST_F(LoggerTest, LogFormat)
{
    CoutCapture capture;

    logger->info("Test message");

    std::string output = capture.getOutput();
    EXPECT_TRUE(output.find("[INFO] Test message") != std::string::npos);
}

TEST_F(LoggerTest, AllLogLevels)
{
    CoutCapture capture;

    // Set to DEBUG to see all messages
    logger->setLogLevel(LogLevel::DEBUG);

    logger->debug("Debug message");
    logger->info("Info message");
    logger->warn("Warning message");
    logger->error("Error message");

    std::string output = capture.getOutput();
    EXPECT_TRUE(output.find("[DEBUG] Debug message") != std::string::npos);
    EXPECT_TRUE(output.find("[INFO] Info message") != std::string::npos);
    EXPECT_TRUE(output.find("[WARN] Warning message") != std::string::npos);
    EXPECT_TRUE(output.find("[ERROR] Error message") != std::string::npos);
}