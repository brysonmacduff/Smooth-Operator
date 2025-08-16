#include "payload_encoder.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(PayloadEncoderTest, PrependHeader)
{
    std::string payload = "This is a payload";
    EXPECT_TRUE(PayloadEncoder::PrependHeader(payload));
    EXPECT_EQ("deadbeef00000011This is a payload", payload);
}

TEST(PayloadEncoderTest, EmptyPayload)
{
    std::string payload;
    EXPECT_FALSE(PayloadEncoder::PrependHeader(payload));
}

} // namespace SmoothOperator::Test