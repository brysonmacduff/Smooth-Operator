#include "protocol.h"

namespace SmoothOperator
{

Header Protocol::BuildHeader(uint32_t payload_size)
{
    uint16_t checksum = 0;

    Header header 
    {
        .prefix = PREFIX,
        .payload_size = payload_size,
        .version = VERSION,
        .checksum = checksum
    };

    std::array<char, sizeof(Header)> header_bytes;
    memcpy(header_bytes.data(),&header,sizeof(Header));

    const std::span<char> header_bytes_view(header_bytes.begin(),header_bytes.end());

    checksum = ComputeChecksum(header_bytes_view);

    header.checksum = checksum;

    return header;
}

bool Protocol::IsHeaderValid(std::span<char> header_bytes)
{
    if(header_bytes.size() != sizeof(Header) || not IsPrefixValid(header_bytes) || not IsVersionValid(header_bytes))
    {
        return false;
    }

    Header header {};
    memcpy(&header, header_bytes.data(), sizeof(Header));

    uint16_t checksum = ComputeChecksum(header_bytes);

    return header.checksum == checksum;
}

uint16_t Protocol::ComputeChecksum(std::span<char> header_bytes)
{
    uint16_t sum = 0;

    // Note that the checksum represents the last two bytes of the header when represented as a byte array. It is not included in computing the checksum.
    for(size_t index = 0; index < header_bytes.size() - sizeof(MAXIMUM_CHECKSUM_VALUE); ++index)
    {
        sum += static_cast<uint8_t>(header_bytes[index]);
    }

    sum %= MAXIMUM_CHECKSUM_VALUE;

    return sum;
}

bool Protocol::IsPrefixValid(std::span<char> header_bytes)
{
    if(header_bytes.size() != sizeof(Header))
    {
        return false;
    }

    std::span<char> prefix_bytes_view = header_bytes.subspan(PREFIX_START_INDEX, sizeof(PREFIX));

    uint32_t prefix = 0;

    memcpy(&prefix, prefix_bytes_view.data(), sizeof(PREFIX));

    return prefix == PREFIX;
}

bool Protocol::IsVersionValid(std::span<char> header_bytes)
{
    if(header_bytes.size() != sizeof(Header))
    {
        return false;
    }

    std::span<char> version_bytes_view = header_bytes.subspan(VERSION_START_INDEX, sizeof(VERSION));

    uint8_t version = 0;

    memcpy(&version, version_bytes_view.data(), sizeof(PREFIX));

    return version == VERSION;
}

} // namespace SmoothOperator