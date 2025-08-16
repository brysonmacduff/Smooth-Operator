#pragma once
#include <string>
#include <iomanip>
#include <optional>
#include <span>

namespace SmoothOperator
{
class Protocol
{
public:
    Protocol() = delete;

    static constexpr unsigned int HEADER_PREFIX = 0xdeadbeef;
    static constexpr unsigned int MAXIMUM_PAYLOAD_SIZE = 0xffffffff;

    static constexpr unsigned int HEADER_PREFIX_START_INDEX = 0;
    static constexpr unsigned int PAYLOAD_SIZE_START_INDEX = 8;

    static constexpr unsigned int HEADER_PREFIX_LENGTH = 8;
    static constexpr unsigned int PAYLOAD_SIZE_LENGTH = 8;
    static constexpr unsigned int TOTAL_HEADER_LENGTH = 16;

    /*!
        \brief Converts an integer to its hex representation as a string.
    */
    static std::string IntegerToHexString(unsigned int number, unsigned int width);
    /*!
        \brief Attempts to convert a hex string representation of a number to a numeric type. An invalid hex string will result in a return value of std::nullopt.
    */
    static std::optional<unsigned long> HexStringToInteger(const std::string& unsigned_hex);
    static std::optional<unsigned long> HexStringToInteger(std::span<char> unsigned_hex_view);
    /*!
        \brief Checks if a header conforms to the protocol specifications.
    */
    static bool IsHeaderFormatValid(const std::span<char>& header);

private:
    static constexpr char STUFFING_CHARACTER = '0';
    static constexpr char MINUS_SIGN = '-';
    static constexpr int HEX_BASE = 16;
    static bool IsHeaderPrefixFormatValid(const std::span<char>& header);
    static bool IsPayloadSizeFormatValid(const std::span<char>& header);
    static bool IsHexStringValid(const std::span<char>& hex_string);
    static bool IsHexStringValid(const std::string& hex_string);
    
};
} // namespace SmoothOperator