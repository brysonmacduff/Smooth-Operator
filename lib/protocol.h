#pragma once
#include <span>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace SmoothOperator
{

struct __attribute__((packed)) Header
{
    uint32_t prefix;
    uint32_t payload_size;
    uint8_t version;
    uint16_t checksum;

    friend std::ostream& operator<<(std::ostream& ostream, const Header& header)
    {
        std::stringstream prefix_stream;
        prefix_stream << std::hex << header.prefix;

        ostream << "[ Prefix: " << prefix_stream.str();
        ostream << ", Payload-Size: " << header.payload_size;
        ostream << ", Version: " << header.version;
        ostream << ", Checksum: " << header.checksum << " ]";

        return ostream;
    }
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
    static bool IsHeaderValid(const Header& header);
    static void ConvertToNetworkEndian(Header& header);
    static void ConvertToLocalEndian(Header& header);

private:

    static constexpr size_t PREFIX_START_INDEX = 0;
    static constexpr size_t VERSION_START_INDEX = 8;

    static bool IsPrefixValid(const Header& header);
    static bool IsVersionValid(const Header& header);
    static uint16_t ComputeChecksum(const Header& header);
    
};
} // namespace SmoothOperator