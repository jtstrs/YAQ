#include "../src/ini_config_reader.h"

#include <gtest/gtest.h>

TEST(IniConfigReader, ReadValidConfig)
{
    std::stringstream ss;
    ss << "key=value\n";
    ss << "key2=value2";
    IniConfigReader reader;
    auto config = reader.read(ss);
    EXPECT_EQ(config["key"], "value");
    EXPECT_EQ(config["key2"], "value2");
}

TEST(IniConfigReader, ReadBadConfig)
{
    std::stringstream ss;
    ss << "key=\n";
    ss << "key2=value";
    IniConfigReader reader;
    EXPECT_THROW(reader.read(ss), std::runtime_error);
}