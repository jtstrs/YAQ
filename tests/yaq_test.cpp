#include "../src/yaq.h"
#include "mocks.h"
#include "gtest/gtest.h"

class ValidConfigTest : public ::testing::Test {
protected:
    std::unordered_map<std::string, std::string> config_;
    void SetUp() override
    {
        config_ = { { "host", "127.0.0.1" }, { "port", "8080" } };
    }
};

TEST(YaqCreateTest, CreateWithValidConfig)
{
    std::unordered_map<std::string, std::string> config = {
        { "host", "127.0.0.1" },
        { "port", "8080" }
    };
    auto yaq = Yaq::create(config);
    EXPECT_NE(yaq, nullptr);
}

// Probably bind to default port???
TEST(YaqCreateTest, CreateWithoutPort)
{
    std::unordered_map<std::string, std::string> config_without_port = {
        { "host", "127.0.0.1" }
    };
    EXPECT_THROW({ auto yaq = Yaq::create(config_without_port); }, std::runtime_error);
}

TEST_F(ValidConfigTest, AcceptConnectionSuccess)
{
    auto yaq = YaqBase<FakeAcceptorSuccess<FakeSocket>, FakeSocket, FakeConnection>::create(config_);
    yaq->set_accepted_callback([this](const boost::system::error_code& error) {
        EXPECT_FALSE(error);
    });
    yaq->run();
}

TEST_F(ValidConfigTest, AcceptConnectionError)
{
    auto yaq = YaqBase<FakeAcceptorError<FakeSocket>, FakeSocket, FakeConnection>::create(config_);
    yaq->set_accepted_callback([this](const boost::system::error_code& error) {
        EXPECT_TRUE(error);
    });
    yaq->run();
}

TEST_F(ValidConfigTest, AcceptConnectionWithoutCallback)
{
    auto yaq = YaqBase<FakeAcceptorSuccess<FakeSocket>, FakeSocket, FakeConnection>::create(config_);
    ASSERT_NO_FATAL_FAILURE(yaq->run());
}