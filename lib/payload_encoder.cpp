#include "payload_encoder.h"

namespace SmoothOperator
{

bool PayloadEncoder::PrependHeader(std::string& payload)
{
    if(payload.size() > Protocol::MAXIMUM_PAYLOAD_SIZE || payload.empty())
    {
        return false;
    }

    const std::string header_prefix_section = GetHeaderPrefixSection();
    const std::string payload_size_section = GetPayloadSizeSection(payload.size());

    payload = header_prefix_section + payload_size_section + payload;

    return true;
}

std::string PayloadEncoder::GetHeaderPrefixSection()
{
    return Protocol::IntegerToHexString(Protocol::HEADER_PREFIX, Protocol::HEADER_PREFIX_LENGTH);
}

std::string PayloadEncoder::GetPayloadSizeSection(unsigned long payload_size)
{
    return Protocol::IntegerToHexString(payload_size, Protocol::PAYLOAD_SIZE_LENGTH);
}

} // namespace SmoothOperator