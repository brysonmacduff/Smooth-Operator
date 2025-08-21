#include "encoder.h"

namespace SmoothOperator
{

std::optional<std::array<char,sizeof(Header)>> Encoder::RequestHeader(std::span<char> payload)
{
    if(payload.empty() || payload.size() > Protocol::MAXIMUM_PAYLOAD_SIZE)
    {
        return std::nullopt;
    }

    const Header header = Protocol::BuildHeader(payload.size());

    std::array<char,sizeof(Header)> header_bytes;

    memcpy(header_bytes.data(),&header,sizeof(Header));

    return header_bytes;
}

} // namespace SmoothOperator