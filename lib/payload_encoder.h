#pragma once
#include "protocol.h"
#include <string>

namespace SmoothOperator
{
class PayloadEncoder
{
public:
    PayloadEncoder() = delete;
    /*!
        \brief This function places a header at the front of the payload.
    */
    static bool PrependHeader(std::string& payload);

private:
    static std::string GetHeaderPrefixSection();
    static std::string GetPayloadSizeSection(unsigned long payload_size);
};
} // namespace SmoothOperator