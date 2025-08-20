#pragma once
#include <span>
#include <cstdint>
#include <cstring>

namespace SmoothOperator
{

struct __attribute__((packed)) Header
{
    uint32_t prefix;
    uint32_t payload_size;
    uint8_t version;
    uint16_t checksum;
};

class Protocol
{
public:
    Protocol() = delete;

    static constexpr uint32_t PREFIX = 0xdeadbeef;
    static constexpr uint32_t MAXIMUM_PAYLOAD_SIZE = 0xffffffff;
    static constexpr uint8_t VERSION = 0x1;
    static constexpr uint16_t MAXIMUM_CHECKSUM_VALUE = 0xffff;

    static Header BuildHeader(uint32_t payload_size);
    static bool IsHeaderValid(std::span<char> header_bytes);
    static uint16_t ComputeChecksum(std::span<char> header_bytes);

private:

    static constexpr size_t PREFIX_START_INDEX = 0;
    static constexpr size_t VERSION_START_INDEX = 8;

    static bool IsPrefixValid(std::span<char> header_bytes);
    static bool IsVersionValid(std::span<char> header_bytes);
    
};
} // namespace SmoothOperator