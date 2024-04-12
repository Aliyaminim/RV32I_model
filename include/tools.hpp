#pragma once

#include <string>
#include <sstream>

template <typename T>
static std::string num2hex(T num) {
    std::stringstream stream;
    stream << std::hex << num;
    return std::string(stream.str());
}