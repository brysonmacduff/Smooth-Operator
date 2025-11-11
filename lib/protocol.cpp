#include "protocol.h"

namespace SmoothOperator
{

Header Protocol::BuildHeader(std::span<char> bytes)
{
    if(bytes.empty() || bytes.size() > MAXIMUM_PAYLOAD_SIZE)
    {
        return {};
    }

    uint16_t checksum = 0;

    Header header 
    {
        .preamble = PREAMBLE,
        .payload_size = static_cast<uint32_t>(bytes.size()),
        .version = VERSION,
        .checksum = checksum
    };

    checksum = ComputeChecksum(header);

    header.checksum = checksum;

    return header;
}

bool Protocol::IsHeaderValid(const Header& header)
{
    if(not IsPrefixValid(header) || not IsVersionValid(header))
    {
        return false;
    }

    const uint16_t checksum = ComputeChecksum(header);

    return header.checksum == checksum;
}

void Protocol::ConvertToNetworkEndian(Header& header)
{
    header.preamble = ntohl(header.preamble);
    header.payload_size = ntohl(header.payload_size);
    header.checksum = ntohs(header.checksum);
}

void Protocol::ConvertToLocalEndian(Header& header)
{
    header.preamble = htonl(header.preamble);
    header.payload_size = htonl(header.payload_size);
    header.checksum = htons(header.checksum);
}

uint16_t Protocol::ComputeChecksum(const Header& header)
{
    std::vector<uint8_t> header_bytes(sizeof(Header));
    memcpy(header_bytes.data(), &header, sizeof(Header));

    uint16_t sum = 0;

    // Note that the checksum represents the last two bytes of the header when represented as a byte array. It is not included in computing the checksum.
    for(size_t index = 0; index < header_bytes.size() - sizeof(MAXIMUM_CHECKSUM_VALUE); ++index)
    {
        sum += header_bytes[index];
    }

    sum %= MAXIMUM_CHECKSUM_VALUE;

    return sum;
}

bool Protocol::IsPrefixValid(const Header& header)
{
    return header.preamble == PREAMBLE;
}

bool Protocol::IsVersionValid(const Header& header)
{
    return header.version == VERSION;
}

} // namespace SmoothOperator