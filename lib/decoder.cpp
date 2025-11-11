#include "decoder.h"

namespace SmoothOperator
{

Decoder::Decoder(unsigned long buffer_size)
{
    m_header_bytes.reserve(sizeof(Header));
    m_payload_bytes.reserve(buffer_size);
}

bool Decoder::Accumulate(std::span<char> bytes)
{
    if(bytes.empty())
    {
        return false;
    }

    bool result = false;

    for (const char& rune : bytes)
    {
        if(not m_current_header_opt.has_value())
        {
            result = m_ProcessHeaderByte(rune);
        }
        else
        {
            result = m_ProcessPayloadByte(rune);
        }

        // Exit right away if something went wrong
        if(not result)
        {
            break;
        }
    }

    m_sticky_error_flag = not result;

    return result;
}

void Decoder::SetPayloadCallback(PayloadCallback callback)
{
    m_payload_callback = std::move(callback);
}

void Decoder::Reset()
{
    m_Clear();
}

bool Decoder::m_ProcessHeaderByte(const char& rune)
{
    // If there is already a header, then this function was called in error
    if(m_current_header_opt.has_value())
    {
        return false;
    }

    m_header_bytes.push_back(rune);
    
    // There are not enough bytes yet to build the header, and this is ok
    if(m_header_bytes.size() < sizeof(Header))
    {
        return true;
    }

    // Build the header now that all the header bytes have been received

    Header header {};
    memcpy(&header, m_header_bytes.data(), sizeof(Header));

    Protocol::ConvertToLocalEndian(header);

    // If the header is not valid, then return false
    if(not Protocol::IsHeaderValid(header))
    {
        return false;
    }

    m_current_header_opt = header;
    return true;
}

bool Decoder::m_ProcessPayloadByte(const char& rune)
{   
    // If the header has not been received already then this function was called in error
    if(not m_current_header_opt.has_value())
    {
        return false;
    }

    m_payload_bytes.push_back(rune);

    if(m_payload_bytes.size() == m_current_header_opt.value().payload_size)
    {
        std::span<char> payload_view(m_payload_bytes.data(), m_payload_bytes.size());
        m_payload_callback(payload_view);
        m_Clear();
    }

    return true;
}

void Decoder::m_Clear()
{
    m_header_bytes.clear();
    m_payload_bytes.clear();
    m_current_header_opt.reset();
}

} // namespace SmoothOperator