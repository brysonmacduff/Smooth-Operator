#include "protocol.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(ProtocolTest, IntegerToHexString)
{
    const unsigned long number = 255;
    const int width = 8;
    const std::string hex_string = Protocol::IntegerToHexString(number, width);
    EXPECT_EQ("000000ff", hex_string);
}

TEST(ProtocolTest, HexStringToInteger)
{
    const std::string hex = "ff";
    const std::optional<unsigned long> number_opt = Protocol::HexStringToInteger(hex);

    EXPECT_TRUE(number_opt.has_value());
    EXPECT_EQ(number_opt, 255);
}

TEST(ProtocolTest, HexSpanToInteger)
{
    std::string hex = "ff";
    std::span<char> hex_view (hex.begin(), hex.end());
    const std::optional<unsigned long> number_opt = Protocol::HexStringToInteger(hex_view);

    EXPECT_TRUE(number_opt.has_value());
    EXPECT_EQ(number_opt, 255);
}

TEST(ProtocolTest, NegativeHexString)
{
    const std::string hex = "-ff";
    const std::optional<unsigned long> number_opt = Protocol::HexStringToInteger(hex);

    EXPECT_FALSE(number_opt.has_value());
}

TEST(ProtocolTest, EmptyHexString)
{
    const std::string hex;
    const std::optional<unsigned long> number_opt = Protocol::HexStringToInteger(hex);

    EXPECT_FALSE(number_opt.has_value());
}

TEST(ProtocolTest, IsHeaderFormatValid)
{
    std::string header = "deadbeef00000011";
    std::span<char> header_view(header.begin(),header.end());
    EXPECT_TRUE(Protocol::IsHeaderFormatValid(header_view));
}

TEST(ProtocolTest, InvalidHeaderPrefix)
{
    std::string header = "deaXbeef00000011";
    std::span<char> header_view(header.begin(),header.end());
    EXPECT_FALSE(Protocol::IsHeaderFormatValid(header_view));
}

TEST(ProtocolTest, InvalidPayloadSize)
{
    std::string header = "deadbeef0000X011";
    std::span<char> header_view(header.begin(),header.end());
    EXPECT_FALSE(Protocol::IsHeaderFormatValid(header_view));
}

TEST(ProtocolTest, EmptyHeader)
{
    std::string header;
    std::span<char> header_view(header.begin(),header.end());
    EXPECT_FALSE(Protocol::IsHeaderFormatValid(header_view));
}

TEST(ProtocolTest, OversizedHeader)
{
    std::string header = "deadbeef000000111";
    std::span<char> header_view(header.begin(),header.end());
    EXPECT_FALSE(Protocol::IsHeaderFormatValid(header_view));
}

} // namespace SmoothOperator::Test