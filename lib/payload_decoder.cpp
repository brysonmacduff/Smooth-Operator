#include "payload_decoder.h"

namespace SmoothOperator
{

PayloadDecoder::PayloadDecoder(unsigned long buffer_size) : m_buffer_size(buffer_size)
{
    m_accumulated_bytes.reserve(m_buffer_size);
}

PayloadDecoder::PayloadDecoder(PayloadDecoder &&other) noexcept
{
    m_accumulated_bytes = std::move(other.m_accumulated_bytes);
    m_payload_callback = std::move(other.m_payload_callback);
    m_error_callback = std::move(other.m_error_callback);
    m_current_payload_size = other.m_current_payload_size;
    m_buffer_size = other.m_buffer_size;
}

bool PayloadDecoder::Accumulate(const std::span<char>& bytes)
{
    bool result = true;

    for (const char &rune : bytes)
    {
        m_accumulated_bytes.push_back(rune);

        const bool is_last_header_byte = m_accumulated_bytes.size() == Protocol::TOTAL_HEADER_LENGTH;
        const bool is_payload_complete = m_current_payload_size > 0 && m_accumulated_bytes.size() == m_current_payload_size + Protocol::TOTAL_HEADER_LENGTH;

        if (is_last_header_byte)
        {
            result = m_ProcessHeader();
        }
        else if (is_payload_complete)
        {
            result = m_ProcessPayload();
        }
    }

    return result;
}

void PayloadDecoder::SetPayloadCallback(PayloadCallback callback)
{
    m_payload_callback = std::move(callback);
}

void PayloadDecoder::SetErrorCallback(ErrorCallback callback)
{
    m_error_callback = std::move(callback);
}

void PayloadDecoder::Reset()
{
    if (m_accumulated_bytes.capacity() > m_buffer_size)
    {
        m_accumulated_bytes.resize(m_buffer_size);
    }

    m_Clear();
}

bool PayloadDecoder::m_ProcessHeader()
{
    if (m_accumulated_bytes.size() != Protocol::TOTAL_HEADER_LENGTH)
    {
        m_error_callback(DecoderError::INTERNAL_PARSING_FAILURE);
        return false;
    }

    auto header_start = m_accumulated_bytes.begin();
    auto header_end = m_accumulated_bytes.begin();
    std::advance(header_end, Protocol::TOTAL_HEADER_LENGTH);
    const std::span<char> header(header_start, header_end);

    if (not Protocol::IsHeaderFormatValid(header))
    {
        m_error_callback(DecoderError::INVALID_HEADER);
        return false;
    }

    return m_ExtractPayloadSize(header);
}

bool PayloadDecoder::m_ProcessPayload()
{
    if (m_accumulated_bytes.size() != m_current_payload_size + Protocol::TOTAL_HEADER_LENGTH)
    {
        m_error_callback(DecoderError::INTERNAL_PARSING_FAILURE);
        return false;
    }

    auto payload_start = m_accumulated_bytes.begin();
    auto payload_end = m_accumulated_bytes.end();
    std::advance(payload_start, Protocol::TOTAL_HEADER_LENGTH);

    const std::span<char> payload(payload_start, payload_end);
    m_payload_callback(payload);
    m_Clear();
    return true;
}

bool PayloadDecoder::m_ExtractPayloadSize(const std::span<char> &header)
{
    if(header.size() != Protocol::TOTAL_HEADER_LENGTH)
    {
        return false;
    }

    const std::span<char> payload_size_section_view = header.subspan(Protocol::PAYLOAD_SIZE_START_INDEX, Protocol::PAYLOAD_SIZE_LENGTH);
    const std::optional<unsigned long> payload_size_opt = Protocol::HexStringToInteger(payload_size_section_view);

    if(not payload_size_opt.has_value())
    {
        return false;
    }

    m_current_payload_size = payload_size_opt.value();

    return true;
}

void PayloadDecoder::m_Clear()
{
    m_current_payload_size = 0;
    m_accumulated_bytes.clear();
}

} // namespace SmoothOperator