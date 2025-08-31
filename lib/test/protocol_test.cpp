#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(ProtocolTest, BuildHeader)
{
    const uint32_t payload_size = 16;
    Header header = Protocol::BuildHeader(payload_size);

    EXPECT_EQ(header.prefix, Protocol::PREFIX);
    EXPECT_EQ(header.payload_size, payload_size);
    EXPECT_EQ(header.version, Protocol::VERSION);
}

TEST(ProtocolTest, ValidHeader)
{
    const uint32_t payload_size = 16;
    const Header header = Protocol::BuildHeader(payload_size);

    EXPECT_TRUE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, InvalidHeaderChecksum)
{
    const uint32_t payload_size = 16;
    Header header = Protocol::BuildHeader(payload_size);

    // Intentionall corrupt the checksum

    header.checksum = 0xff;

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, InvalidHeaderPrefix)
{
    const uint32_t payload_size = 16;
    Header header = Protocol::BuildHeader(payload_size);

    // Intentionall corrupt the prefix
    header.prefix = Protocol::PREFIX + 1;

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, InvalidHeaderVersion)
{
    const uint32_t payload_size = 16;
    Header header = Protocol::BuildHeader(payload_size);

    // Intentionally corrupt the version

    header.version = Protocol::VERSION + 1;

    EXPECT_FALSE(Protocol::IsHeaderValid(header));
}

TEST(ProtocolTest, ConvertToNetworkEndian)
{
    Header header 
    {
        .prefix = Protocol::PREFIX,
        .payload_size = 0x89ABCDEF,
        .version = Protocol::VERSION,
        .checksum = 0xFEDC
    };

    Protocol::ConvertToNetworkEndian(header);

    EXPECT_EQ(header.version,Protocol::VERSION);
    EXPECT_EQ(header.prefix,0xEFBEADDE);
    EXPECT_EQ(header.payload_size, 0xEFCDAB89);
    EXPECT_EQ(header.checksum, 0xDCFE);
}

TEST(ProtocolTest, ConvertToLocalEndian)
{
    Header header 
    {
        .prefix = Protocol::PREFIX,
        .payload_size = 0x89ABCDEF,
        .version = Protocol::VERSION,
        .checksum = 0xFEDC
    };

    Protocol::ConvertToNetworkEndian(header);

     std::cout << header << "\n";

    Protocol::ConvertToLocalEndian(header);

    std::cout << header << "\n";

    EXPECT_EQ(header.version,Protocol::VERSION);
    EXPECT_EQ(header.prefix,Protocol::PREFIX);
    EXPECT_EQ(header.payload_size, 0x89ABCDEF);
    EXPECT_EQ(header.checksum, 0xFEDC);
}

} // namespace SmoothOperator::Test