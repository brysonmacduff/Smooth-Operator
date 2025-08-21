#include "decoder.h"

namespace SmoothOperator
{

Decoder::Decoder(unsigned long buffer_size) : m_buffer_size(buffer_size)
{
    m_accumulated_bytes.reserve(m_buffer_size);
}

Decoder::Decoder(Decoder &&other) noexcept
: m_accumulated_bytes(std::move(other.m_accumulated_bytes))
, m_payload_callback(std::move(other.m_payload_callback))
, m_current_payload_size(other.m_current_payload_size)
, m_buffer_size(other.m_buffer_size)
{
}

bool Decoder::Accumulate(const std::span<char>& bytes)
{
    if(bytes.empty())
    {
        return false;
    }

    bool result = true;

    for (const char &rune : bytes)
    {
        m_accumulated_bytes.push_back(rune);

        const bool is_last_header_byte = m_accumulated_bytes.size() == sizeof(Header);
        const bool is_last_payload_byte = m_current_payload_size > 0 && m_accumulated_bytes.size() == sizeof(Header) + m_current_payload_size;

        if (is_last_header_byte)
        {
            result = m_ProcessHeader();
        }
        else if (is_last_payload_byte)
        {
            result = m_ProcessPayload();
        }

        if(not result)
        {
            break;
        }
    }

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

bool Decoder::m_ProcessHeader()
{
   if(m_accumulated_bytes.size() != sizeof(Header))
   {
        return false;
   }

   Header header {};
   memcpy(&header, m_accumulated_bytes.data(), sizeof(Header));

   std::span<char> header_bytes_view(m_accumulated_bytes.begin(),m_accumulated_bytes.end());
   
   if(not Protocol::IsHeaderValid(header_bytes_view))
   {
        return false;
   }

   m_current_payload_size = header.payload_size;
   return true;
}

bool Decoder::m_ProcessPayload()
{
   if(m_accumulated_bytes.size() != sizeof(Header) + m_current_payload_size)
   {
        return false;
   }

   auto payload_start_it = m_accumulated_bytes.begin();
   std::advance(payload_start_it, sizeof(Header));

   std::span<char> payload_view(payload_start_it, m_accumulated_bytes.end());

   m_payload_callback(payload_view);

   m_Clear();

   return true;
}

void Decoder::m_Clear()
{
    m_current_payload_size = 0;
    m_accumulated_bytes.clear();
}

} // namespace SmoothOperator