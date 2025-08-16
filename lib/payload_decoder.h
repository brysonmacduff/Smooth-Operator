#pragma once
#include "decoder_error.h"
#include "protocol.h"
#include <functional>
#include <vector>
#include <span>

namespace SmoothOperator
{
class PayloadDecoder
{
public:

    using PayloadCallback = std::function<void(const std::span<char>& payload)>;
    using ErrorCallback = std::function<void(const DecoderError& error)>;

    PayloadDecoder(const PayloadDecoder&) = delete;
    PayloadDecoder& operator=(const PayloadDecoder&) = delete;
    PayloadDecoder(unsigned long buffer_size = DEFAULT_BUFFER_SIZE);
    PayloadDecoder(PayloadDecoder&& other) noexcept;
    /*!
        \brief Accumulates bytes to construct a payload. Returns false if a parsing error occurs, otherwise returns true.
    */
    bool Accumulate(const std::span<char>& bytes);
    /*!
        \brief Set a callback function to receive payloads. The payload callback is triggered when enough bytes have accumulated to complete a payload.
    */
    void SetPayloadCallback(PayloadCallback callback);
    /*!
        \brief Set a callback function to receive notifications when parsing errors occur.
    */
    void SetErrorCallback(ErrorCallback callback);
    /*!
        \brief Resets stateful variables. Use Reset to start over in the event of a parsing error.
    */
    void Reset();

private:
    static constexpr unsigned long DEFAULT_BUFFER_SIZE = 1024;
    unsigned long m_buffer_size;
    std::vector<char> m_accumulated_bytes;
    PayloadCallback m_payload_callback = [](const std::span<char>& payload){(void)payload;};
    ErrorCallback m_error_callback = [](const DecoderError& error){(void)error;};
    unsigned long m_current_payload_size { 0 };

    bool m_ProcessHeader();
    bool m_ProcessPayload();
    bool m_ExtractPayloadSize(const std::span<char>& header);
    void m_Clear();
};
} // namespace SmoothOperator