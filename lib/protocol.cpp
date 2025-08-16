#include "protocol.h"

namespace SmoothOperator
{
std::string Protocol::IntegerToHexString(unsigned int number, unsigned int width)
{
    std::stringstream hex_stream;
    hex_stream <<  std::setw(width) << std::setfill(STUFFING_CHARACTER) << std::hex << number;
    return hex_stream.str();
}

std::optional<unsigned long> Protocol::HexStringToInteger(const std::string& unsigned_hex)
{
    if(not IsHexStringValid(unsigned_hex))
    {
        return std::nullopt;
    }

    return std::stoul(unsigned_hex, nullptr, HEX_BASE);
}

std::optional<unsigned long> Protocol::HexStringToInteger(std::span<char> unsigned_hex_view)
{
    const std::string unsigned_hex (unsigned_hex_view.begin(), unsigned_hex_view.end());

    if(not IsHexStringValid(unsigned_hex))
    {
        return std::nullopt;
    }

    return std::stoul(unsigned_hex, nullptr, HEX_BASE);
}

bool Protocol::IsHeaderFormatValid(const std::span<char>& header)
{
    if(header.size() != TOTAL_HEADER_LENGTH)
    {
        return false;
    }

    return IsHeaderPrefixFormatValid(header) && IsPayloadSizeFormatValid(header);
}

bool Protocol::IsHeaderPrefixFormatValid(const std::span<char>& header)
{
    if(header.size() != TOTAL_HEADER_LENGTH)
    {
        return false;
    }

    const std::span<char> header_prefix = header.subspan(HEADER_PREFIX_START_INDEX, HEADER_PREFIX_LENGTH);

    return std::string(header_prefix.begin(),header_prefix.end()) == IntegerToHexString(HEADER_PREFIX, HEADER_PREFIX_LENGTH);
}

bool Protocol::IsPayloadSizeFormatValid(const std::span<char>& header)
{
    if(header.size() != TOTAL_HEADER_LENGTH)
    {
        return false;
    }

    const std::span<char> payload_size = header.subspan(PAYLOAD_SIZE_START_INDEX, PAYLOAD_SIZE_LENGTH);

    return IsHexStringValid(payload_size);
}

bool Protocol::IsHexStringValid(const std::span<char>& hex_string)
{
    if(hex_string.empty() || hex_string.front() == MINUS_SIGN)
    {
        return false;
    }

    for(const char& rune : hex_string)
    {
        if(std::isxdigit(rune) == 0)
        {
            return false;
        }
    }

    return true;
}

bool Protocol::IsHexStringValid(const std::string& hex_string)
{
    if(hex_string.empty() || hex_string.front() == MINUS_SIGN)
    {
        return false;
    }

    for(const char& rune : hex_string)
    {
        if(std::isxdigit(rune) == 0)
        {
            return false;
        }
    }

    return true;
}

} // namespace SmoothOperator