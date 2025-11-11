#include "encoder.h"
#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(EncoderTest, MinimumPayloadSize)
{
    std::string payload = "a";
    std::span<char> payload_view(payload.begin(), payload.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

    Header header {};
    memcpy(&header, header_bytes.data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    EXPECT_TRUE(Protocol::IsHeaderValid(header));
}

} // namespace SmoothOperator::Test