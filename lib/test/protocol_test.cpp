#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(ProtocolTest, BuildHeader)
{
    const uint32_t payload_size = 16;
    std::vector<char> payload(payload_size);
    std::span<char> payload_view(payload);
    Header header = Protocol::BuildHeader(payload_view);

    EXPECT_EQ(header.preamble, Protocol::PREAMBLE);
    EXPECT_EQ(header.payload_size, payload_size);
    EXPECT_EQ(header.version, Protocol::VERSION);
}

TEST(ProtocolTest, ValidHeader)
{
    const uint32_t payload_size = 16;
    std::vector<char> payload(payload_size);
    std::span<char> payload_view(payload);
    Header header = Protocol::BuildHeader(payload_view);

    EXPECT_TRUE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, InvalidHeaderChecksum)
{
    const uint32_t payload_size = 16;
    std::vector<char> payload(payload_size);
    std::span<char> payload_view(payload);
    Header header = Protocol::BuildHeader(payload_view);

    // Intentionall corrupt the checksum

    header.checksum = 0xff;

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, InvalidHeaderPrefix)
{
    const uint32_t payload_size = 16;
    std::vector<char> payload(payload_size);
    std::span<char> payload_view(payload);
    Header header = Protocol::BuildHeader(payload_view);

    // Intentionall corrupt the prefix
    header.preamble = Protocol::PREAMBLE + 1;

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, InvalidHeaderVersion)
{
    const uint32_t payload_size = 16;
    std::vector<char> payload(payload_size);
    std::span<char> payload_view(payload);
    Header header = Protocol::BuildHeader(payload_view);

    // Intentionally corrupt the version

    header.version = Protocol::VERSION + 1;

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, ConvertToNetworkEndian)
{
    Header header 
    {
        .preamble = Protocol::PREAMBLE,
        .payload_size = 0x89ABCDEF,
        .version = Protocol::VERSION,
        .checksum = 0xFEDC
    };

    Protocol::ConvertToNetworkEndian(header);

    EXPECT_EQ(header.version,Protocol::VERSION);
    EXPECT_EQ(header.preamble,0xEFBEADDE);
    EXPECT_EQ(header.payload_size, 0xEFCDAB89);
    EXPECT_EQ(header.checksum, 0xDCFE);
}

TEST(ProtocolTest, ConvertToLocalEndian)
{
    Header header 
    {
        .preamble = Protocol::PREAMBLE,
        .payload_size = 0x89ABCDEF,
        .version = Protocol::VERSION,
        .checksum = 0xFEDC
    };

    Protocol::ConvertToNetworkEndian(header);

     std::cout << header << "\n";

    Protocol::ConvertToLocalEndian(header);

    std::cout << header << "\n";

    EXPECT_EQ(header.version,Protocol::VERSION);
    EXPECT_EQ(header.preamble,Protocol::PREAMBLE);
    EXPECT_EQ(header.payload_size, 0x89ABCDEF);
    EXPECT_EQ(header.checksum, 0xFEDC);
}

} // namespace SmoothOperator::Test