#pragma once
#include "protocol.h"
#include <span>
#include <array>
#include <cstring>
#include <optional>

namespace SmoothOperator
{
class Encoder
{
public:
    Encoder() = delete;
    /*!
        \brief This function returns a header byte array based on the provided payload.
        \warning Returns an empty array if the payload is empty or exceeds the maximum allowable size.
    */
    static std::array<char,sizeof(Header)> Encode(std::span<char> payload);

};
} // namespace SmoothOperator