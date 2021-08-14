#pragma once

#include <string>
#include <functional>

namespace utf8help
{
    void RTrim(std::string& str);

    [[nodiscard]] std::string Transform(const std::string& str, std::function<char32_t(char32_t)> func);
    [[nodiscard]] std::string Upper(const std::string& str);
}