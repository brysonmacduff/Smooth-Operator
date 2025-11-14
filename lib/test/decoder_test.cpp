#include "decoder.h"
#include "encoder.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(DecoderTest, RejectEmptyPayload)
{
    Decoder decoder;

    std::string payload;
    std::span<char> payload_view(payload);

    EXPECT_FALSE(decoder.Accumulate(payload_view));
}

TEST(DecoderTest, AccumulatePayload)
{
    std::string payload = "This is a payload";
    std::span<char> payload_view(payload.begin(),payload.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);
    std::span<char> header_view(header_bytes.begin(),header_bytes.end());

    Decoder decoder;
    bool is_payload_callback_activated = false;

    decoder.SetPayloadCallback([&](std::span<char> received_payload)
    {
        EXPECT_EQ(payload, std::string(received_payload.begin(),received_payload.end()));
        is_payload_callback_activated = true;
    });

    EXPECT_TRUE(decoder.Accumulate(header_view));
    EXPECT_TRUE(decoder.Accumulate(payload_view));
    EXPECT_TRUE(is_payload_callback_activated);
}

TEST(DecoderTest, AccumulateMultiplePayloads)
{
    const int payload_count = 100;
    std::vector<std::string> payloads(payload_count);


    Decoder decoder;
    int payload_callback_activations = 0;

    decoder.SetPayloadCallback([&payload_callback_activations, &payloads](std::span<char> received_payload)
    {
        EXPECT_EQ(payloads[payload_callback_activations], std::string(received_payload.begin(),received_payload.end()));
        ++payload_callback_activations;
    });

    for(unsigned long count = 0; count < payloads.size(); ++count)
    {
        payloads[count] = "This is a payload "+std::to_string(count);

        const std::span<char> payload_view(payloads[count]);
        
        std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

        const std::span<char> header_view (header_bytes);
        
        EXPECT_TRUE(decoder.Accumulate(header_view));
        EXPECT_TRUE(decoder.Accumulate(payload_view));
    }

    EXPECT_EQ(payload_callback_activations, payload_count);
}

TEST(DecoderTest, RecoverFromParsingError)
{
    std::string payload_content = "This is a payload";
    std::span<char> payload_view(payload_content.begin(),payload_content.end());

    std::array<char, sizeof(Header)> header_bytes = Encoder::Encode(payload_view);

    std::span<char> header_view(header_bytes);

    Decoder decoder;
    bool is_payload_callback_activated = false;

    decoder.SetPayloadCallback([&](std::span<char> payload)
    {
        EXPECT_EQ(payload_content, std::string(payload.begin(),payload.end()));
        is_payload_callback_activated = true;
    });

    EXPECT_FALSE(decoder.IsStuck());

    // ACCUMULATE INVALID HEADER SEQUENCE //

    EXPECT_FALSE(decoder.Accumulate(payload_view));

    // RECOVER FROM PARSING ERROR //

    EXPECT_TRUE(decoder.IsStuck());
    decoder.Reset();

    EXPECT_TRUE(decoder.Accumulate(header_view));
    EXPECT_TRUE(decoder.Accumulate(payload_view));
    EXPECT_TRUE(is_payload_callback_activated);
    EXPECT_FALSE(decoder.IsStuck());
}

} // namespace SmoothOperator::Test