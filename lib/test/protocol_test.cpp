#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(ProtocolTest, BuildHeader)
{
    const uint32_t payload_size = 16;
    const Header header = Protocol::BuildHeader(payload_size);

    EXPECT_EQ(header.prefix, Protocol::PREFIX);
    EXPECT_EQ(header.payload_size, payload_size);
    EXPECT_EQ(header.version, Protocol::VERSION);
}

TEST(ProtocolTest, ValidHeader)
{
    const uint32_t payload_size = 16;
    const Header header = Protocol::BuildHeader(payload_size);

    std::vector<char> header_bytes(sizeof(Header));

    memcpy(header_bytes.data(), &header, sizeof(Header));

    std::span<char> header_bytes_view(header_bytes.begin(),header_bytes.end());

    EXPECT_TRUE(Protocol::IsHeaderValid(header_bytes_view));
}

TEST(ProtocolTest, InvalidHeaderChecksum)
{
    const uint32_t payload_size = 16;
    const Header header = Protocol::BuildHeader(payload_size);

    std::vector<char> header_bytes(sizeof(Header));

    memcpy(header_bytes.data(), &header, sizeof(Header));

    // Intentionall corrupt the checksum

    const int checksum_start_index = 9;

    header_bytes[checksum_start_index] = 0xff;

    std::span<char> header_bytes_view(header_bytes.begin(),header_bytes.end());

    EXPECT_FALSE(Protocol::IsHeaderValid(header_bytes_view));
}

TEST(ProtocolTest, InvalidHeaderPrefix)
{
    const uint32_t payload_size = 16;
    const Header header = Protocol::BuildHeader(payload_size);

    std::vector<char> header_bytes(sizeof(Header));

    memcpy(header_bytes.data(), &header, sizeof(Header));

    // Intentionall corrupt the checksum

    const int checksum_start_index = 0;

    header_bytes[checksum_start_index] = 0x0;

    std::span<char> header_bytes_view(header_bytes.begin(),header_bytes.end());

    EXPECT_FALSE(Protocol::IsHeaderValid(header_bytes_view));
}

TEST(ProtocolTest, InvalidHeaderVersion)
{
    const uint32_t payload_size = 16;
    const Header header = Protocol::BuildHeader(payload_size);

    std::vector<char> header_bytes(sizeof(Header));

    memcpy(header_bytes.data(), &header, sizeof(Header));

    // Intentionall corrupt the checksum

    const int checksum_start_index = 4;

    header_bytes[checksum_start_index] = 0x0;

    std::span<char> header_bytes_view(header_bytes.begin(),header_bytes.end());

    EXPECT_FALSE(Protocol::IsHeaderValid(header_bytes_view));
}

} // namespace SmoothOperator::Test