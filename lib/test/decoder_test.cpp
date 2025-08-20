#include "decoder.h"
#include "encoder.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(PayloadDecoderTest, AccumulatePayload)
{
    std::string payload = "This is a payload";
    std::span<char> payload_view(payload.begin(),payload.end());

    std::optional<std::array<char, sizeof(Header)>> header_opt = Encoder::RequestHeader(payload_view);

    EXPECT_TRUE(header_opt.has_value());

    std::span<char> header_view(header_opt.value().begin(),header_opt.value().end());

    Decoder decoder;
    bool is_payload_callback_activated = false;

    decoder.SetPayloadCallback([&](const std::span<char>& _payload)
    {
        EXPECT_EQ(payload, std::string(_payload.begin(),_payload.end()));
        is_payload_callback_activated = true;
    });

    EXPECT_TRUE(decoder.Accumulate(header_view));
    EXPECT_TRUE(decoder.Accumulate(payload_view));
    EXPECT_TRUE(is_payload_callback_activated);
}

TEST(PayloadDecoderTest, AccumulatePayloads)
{
    const int payload_count = 100;
    std::vector<std::string> payloads(payload_count);

    Decoder decoder;
    int payload_callback_activations = 0;

    decoder.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ(payloads[payload_callback_activations], std::string(payload.begin(),payload.end()));
        ++payload_callback_activations;
    });

    for(unsigned long count = 0; count < payloads.size(); ++count)
    {
        std::string payload = "This is a payload "+std::to_string(count);
        payloads[count] = payload;

        const std::span<char> payload_view(payload.begin(), payload.end());
        
        std::optional<std::array<char, sizeof(Header)>> header_opt = Encoder::RequestHeader(payload_view);

        EXPECT_TRUE(header_opt.has_value());

        const std::span<char> header_view (header_opt.value().begin(),header_opt.value().end());
        
        EXPECT_TRUE(decoder.Accumulate(header_view));
        EXPECT_TRUE(decoder.Accumulate(payload_view));
    }

    EXPECT_EQ(payload_callback_activations, payload_count);
}

TEST(PayloadDecoderTest, RecoverFromParsingError)
{
    std::string payload_content = "This is a payload";
    std::span<char> payload_view(payload_content.begin(),payload_content.end());

    std::optional<std::array<char, sizeof(Header)>> header_opt = Encoder::RequestHeader(payload_view);

    EXPECT_TRUE(header_opt.has_value());

    std::span<char> header_view(header_opt.value().begin(),header_opt.value().end());

    Decoder decoder;
    bool is_payload_callback_activated = false;

    decoder.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ(payload_content, std::string(payload.begin(),payload.end()));
        is_payload_callback_activated = true;
    });

    // ACCUMULATE INVALID HEADER SEQUENCE //

    EXPECT_FALSE(decoder.Accumulate(payload_view));

    // RECOVER FROM PARSING ERROR //

    decoder.Reset();

    EXPECT_TRUE(decoder.Accumulate(header_view));
    EXPECT_TRUE(decoder.Accumulate(payload_view));
    EXPECT_TRUE(is_payload_callback_activated);
}

TEST(PayloadDecoderTest, MoveConstructor)
{
    std::string payload_content = "This is a payload";
    std::span<char> payload_view(payload_content.begin(),payload_content.end());

    std::optional<std::array<char, sizeof(Header)>> header_opt = Encoder::RequestHeader(payload_view);

    EXPECT_TRUE(header_opt.has_value());

    std::span<char> header_view(header_opt.value().begin(),header_opt.value().end());

    Decoder decoder1;
    bool is_payload_callback_activated = false;

    decoder1.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ(std::string(payload.begin(),payload.end()), payload_content);
        is_payload_callback_activated = true;
    });

    EXPECT_TRUE(decoder1.Accumulate(header_view));

    Decoder decoder2(std::move(decoder1));

    EXPECT_TRUE(decoder2.Accumulate(payload_view));

    EXPECT_TRUE(is_payload_callback_activated);
}

} // namespace SmoothOperator::Test