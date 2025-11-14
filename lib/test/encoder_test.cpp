#include "encoder.h"
#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(EncoderTest, EncodeHeaderFromMinimumSizedPayload)
{
    std::string payload = "a";
    std::span<char> payload_view(payload.begin(), payload.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

    Header header {};
    memcpy(&header, header_bytes.data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    EXPECT_TRUE(Protocol::IsHeaderValid(header));
}

// This test requires the allocation of a large array, which is slow.
#if 0
TEST(EncoderTest, EncodeHeaderFromMaximumSizedPayload)
{
    const size_t payload_size = Protocol::MAXIMUM_PAYLOAD_SIZE;
    std::string payload ("a",payload_size);
    std::span<char> payload_view(payload.begin(), payload.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

    Header header {};
    memcpy(&header, header_bytes.data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    EXPECT_TRUE(Protocol::IsHeaderValid(header));
}
#endif

// This test requires the allocation of a large array, which is slow.
#if 0
TEST(EncoderTest, FailToEncodeHeaderFromOversizedPaylaod)
{
    const size_t payload_size = Protocol::MAXIMUM_PAYLOAD_SIZE+1;
    std::string payload ("a",payload_size);
    std::span<char> payload_view(payload.begin(), payload.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

    Header header {};
    memcpy(&header, header_bytes.data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}
#endif

TEST(EncoderTest, FailToEncodeHeaderFromEmptyPaylaod)
{
    std::string payload;
    std::span<char> payload_view(payload.begin(), payload.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

    Header header {};
    memcpy(&header, header_bytes.data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

} // namespace SmoothOperator::Test