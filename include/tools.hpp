#pragma once

#include <string>
#include <sstream>

template <typename T>
std::string num2hex(T num) {
    std::stringstream stream;
    stream << std::hex << num;
    return stream.str();
}