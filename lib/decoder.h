#pragma once
#include "protocol.h"
#include <functional>
#include <vector>
#include <span>
#include <memory>
#include <optional>

namespace SmoothOperator
{
class Decoder
{
public:

    using PayloadCallback = std::function<void(std::span<char> payload)>;

    Decoder(const Decoder&) = delete;
    Decoder& operator=(const Decoder&) = delete;
    Decoder(unsigned long buffer_size = DEFAULT_BUFFER_SIZE);
    Decoder(Decoder&& other) = default;
    /*!
        \brief Accumulates bytes to construct a payload. Returns false if a parsing error occurs, otherwise returns true.
    */
    bool Accumulate(std::span<char> bytes);
    /*!
        \brief Set a callback function to receive payloads. The payload callback is triggered when enough bytes have accumulated to complete a payload.
    */
    void SetPayloadCallback(PayloadCallback callback);
    /*!
        \brief Resets stateful variables. Use Reset to start over in the event of a parsing error.
    */
    void Reset();
    /**
     * Returns whether the Decoder is currently in an erroneous state due to a parsing error.
     */
    bool IsStuck() const { return m_sticky_error_flag; };

private:
    static constexpr unsigned long DEFAULT_BUFFER_SIZE = 1024;
    std::vector<char> m_header_bytes;
    std::vector<char> m_payload_bytes;
    PayloadCallback m_payload_callback = [](std::span<char> payload){(void)payload;};
    bool m_sticky_error_flag { false };
    std::optional<Header> m_current_header_opt { std::nullopt };

    bool m_ProcessHeaderByte(const char& rune);
    bool m_ProcessPayloadByte(const char& rune);
    void m_Clear();
};
} // namespace SmoothOperator