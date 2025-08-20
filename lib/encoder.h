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
        \brief This function returns a header byte array based on the provided payload
    */
    static std::optional<std::array<char,sizeof(Header)>> RequestHeader(std::span<char> payload);

};
} // namespace SmoothOperator