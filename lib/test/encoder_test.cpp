#include "encoder.h"
#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(EncoderTest, EmptyPayload)
{
    std::string payload;
    std::span<char> payload_view(payload.begin(), payload.end());

    const std::optional<std::array<char, sizeof(Header)>> header_opt = Encoder::RequestHeader(payload_view);

    EXPECT_FALSE(header_opt.has_value());
}

TEST(EncoderTest, MinimumPayloadSize)
{
    std::string payload = "a";
    std::span<char> payload_view(payload.begin(), payload.end());

    std::optional<std::array<char, sizeof(Header)>> header_opt = Encoder::RequestHeader(payload_view);

    EXPECT_TRUE(header_opt.has_value());

    Header header {};
    memcpy(&header, header_opt.value().data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    EXPECT_TRUE(Protocol::IsHeaderValid(header));
}

} // namespace SmoothOperator::Test