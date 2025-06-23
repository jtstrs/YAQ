#include "../src/yaq.h"
#include "mocks.h"
#include "gtest/gtest.h"
#include <stdexcept>

class ValidConfigTest : public ::testing::Test {
protected:
    std::unordered_map<std::string, std::string> config_;
    void SetUp() override
    {
        config_ = { { "host", "127.0.0.1" }, { "port", "8080" } };
    }
};

using FakeNetwork = NetworkBase<FakeAcceptorSuccess<FakeSocket>, FakeSocket, FakeConnection>;

TEST_F(ValidConfigTest, CreateYaq)
{
    auto yaq = YaqBase<FakeNetwork, FakeProtocol>::create(config_);
    ASSERT_TRUE(yaq);
}

TEST_F(ValidConfigTest, RunYaq)
{
    auto yaq = YaqBase<FakeNetwork, FakeProtocol>::create(config_);
    ASSERT_TRUE(yaq);
    ASSERT_NO_FATAL_FAILURE({ yaq->run(); });
}

TEST(BadConfigTest, CreateYaq)
{
    std::unordered_map<std::string, std::string> config_ = { { "host", "127.0.0.1" }, { "port", "not_a_port" } };
    ASSERT_THROW((YaqBase<FakeNetwork, FakeProtocol>::create(config_)), std::runtime_error);
}

TEST_F(ValidConfigTest, StopYaq)
{
    auto yaq = YaqBase<FakeNetwork, FakeProtocol>::create(config_);
    ASSERT_TRUE(yaq);
    ASSERT_NO_FATAL_FAILURE({ yaq->stop(); });
}

TEST_F(ValidConfigTest, HandleNewConnection)
{
    auto yaq = YaqBase<FakeNetwork, FakeProtocol>::create(config_);
    ASSERT_TRUE(yaq);
    ASSERT_NO_FATAL_FAILURE({ yaq->run(); });
}

using FakeNetworkWithMessage = NetworkBase<FakeAcceptorSuccess<FakeSocketReceiveSuccess>, FakeSocketReceiveSuccess, FakeConnection>;

TEST_F(ValidConfigTest, HandleMessage)
{
    auto yaq = YaqBase<FakeNetworkWithMessage, FakeProtocol>::create(config_);
    ASSERT_TRUE(yaq);
    ASSERT_NO_FATAL_FAILURE({ yaq->run(); });
}
