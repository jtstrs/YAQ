#include "../src/acceptor.h"
#include "../src/yaq.h"
#include "gtest/gtest.h"

class YaqTest : public ::testing::Test {
protected:
    std::unique_ptr<Yaq> yaq_;
    std::unordered_map<std::string, std::string> config_;
    void SetUp() override
    {
        config_ = { { "host", "127.0.0.1" }, { "port", "8080" } };
        yaq_ = Yaq::create(config_);
    }
};

TEST(Yaq, CreateWithValidConfig)
{
    std::unordered_map<std::string, std::string> config = {
        { "host", "127.0.0.1" },
        { "port", "8080" }
    };
    auto yaq = Yaq::create(config);
    EXPECT_NE(yaq, nullptr);
}

// Probably bind to default port???
TEST(Yaq, CreateWithoutPort)
{
    std::unordered_map<std::string, std::string> config_without_port = {
        { "host", "127.0.0.1" }
    };
    EXPECT_THROW({ auto yaq = Yaq::create(config_without_port); }, std::runtime_error);
}