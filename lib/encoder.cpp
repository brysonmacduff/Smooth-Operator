#include "encoder.h"

namespace SmoothOperator
{

std::array<char,sizeof(Header)> Encoder::Encode(std::span<char> payload)
{
    // Return an empty header if there is a problem with the payload
    if(payload.empty() || payload.size() > Protocol::MAXIMUM_PAYLOAD_SIZE)
    {
        return {};
    }

    Header header = Protocol::BuildHeader(payload);

    Protocol::ConvertToNetworkEndian(header);

    std::array<char,sizeof(Header)> header_bytes;

    memcpy(header_bytes.data(),&header,sizeof(Header));

    return header_bytes;
}

} // namespace SmoothOperator