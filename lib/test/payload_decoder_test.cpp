#include "payload_decoder.h"
#include "payload_encoder.h"
#include <gtest/gtest.h>

namespace SmoothOperator::Test
{

TEST(PayloadDecoderTest, AccumulatePayload)
{
    std::string payload = "deadbeef00000011This is a payload";
    std::span<char> payload_view(payload.begin(),payload.end());

    PayloadDecoder decoder;
    bool is_payload_callback_activated = false;

    decoder.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ("This is a payload", std::string(payload.begin(),payload.end()));
        is_payload_callback_activated = true;
    });

    EXPECT_TRUE(decoder.Accumulate(payload));
    EXPECT_TRUE(is_payload_callback_activated);
}

TEST(PayloadDecoderTest, AccumulateFragmentedPayload)
{
    std::string payload_fragment1 = "deadbeef000";
    std::string payload_fragment2 = "00011This is a payload";
    const std::span<char> fragment_view1(payload_fragment1.begin(),payload_fragment1.end());
    const std::span<char> fragment_view2(payload_fragment2.begin(),payload_fragment2.end());

    PayloadDecoder decoder;
    bool is_payload_callback_activated = false;

    decoder.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ("This is a payload", std::string(payload.begin(),payload.end()));
        is_payload_callback_activated = true;
    });

    EXPECT_TRUE(decoder.Accumulate(fragment_view1));
    EXPECT_TRUE(decoder.Accumulate(fragment_view2));
    EXPECT_TRUE(is_payload_callback_activated);
}

TEST(PayloadDecoderTest, AccumulatePayloads)
{
    const int payload_count = 100;
    std::vector<std::string> payloads(payload_count);

    PayloadDecoder decoder;
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
        EXPECT_TRUE(PayloadEncoder::PrependHeader(payload));
        const std::span<char> payload_view(payload.begin(), payload.end());
        EXPECT_TRUE(decoder.Accumulate(payload_view));
    }

    EXPECT_EQ(payload_callback_activations, payload_count);
}

TEST(PayloadDecoderTest, RecoverFromParsingError)
{
    const std::string payload_content = "This is a payload";
    const std::string full_payload = "deadbeef00000011This is a payload";
    std::string first_half = full_payload.substr(0, full_payload.size()/2);
    std::string second_half = full_payload.substr(full_payload.size()/2, full_payload.size());

    PayloadDecoder decoder;
    bool is_payload_callback_activated = false;
    bool is_error_callback_activated = false;

    decoder.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ(payload_content, std::string(payload.begin(),payload.end()));
        is_payload_callback_activated = true;
    });
    
    decoder.SetErrorCallback([&](const DecoderError& error)
    {
        EXPECT_EQ(error,DecoderError::INVALID_HEADER);
        is_error_callback_activated = true;
    });

    // *** ACCUMULATE INVALID HEADER SEQUENCE ***

    EXPECT_FALSE(decoder.Accumulate(second_half));

    // *** RECOVER FROM PARSING ERROR ***

    decoder.Reset();

    EXPECT_TRUE(decoder.Accumulate(first_half));
    EXPECT_TRUE(decoder.Accumulate(second_half));
    EXPECT_TRUE(is_payload_callback_activated);
    EXPECT_TRUE(is_error_callback_activated);
}

TEST(PayloadDecoderTest, MoveConstructor)
{
    const std::string payload_content = "This is a payload";
    const std::string full_payload = "deadbeef00000011This is a payload";
    std::string first_half = full_payload.substr(0, full_payload.size()/2);
    std::string second_half = full_payload.substr(full_payload.size()/2, full_payload.size());

    PayloadDecoder decoder1;
    bool is_payload_callback_activated = false;

    decoder1.SetPayloadCallback([&](const std::span<char>& payload)
    {
        EXPECT_EQ(std::string(payload.begin(),payload.end()), payload_content);
        is_payload_callback_activated = true;
    });

    decoder1.Accumulate(first_half);

    PayloadDecoder decoder2(std::move(decoder1));

    decoder2.Accumulate(second_half);

    EXPECT_TRUE(is_payload_callback_activated);
}

} // namespace SmoothOperator::Test