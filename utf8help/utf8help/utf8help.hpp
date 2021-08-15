#pragma once

#include <string>
#include <functional>
#include <utf8.h>

namespace utf8help
{
    std::string RTrimSpace(const std::string& str);

    [[nodiscard]] std::string Transform(const std::string& str, std::function<char32_t(char32_t)> func);
    [[nodiscard]] std::string Upper(const std::string& str);

    [[nodiscard]] std::string ReadString(utf8::iterator<std::string::const_iterator>& it, int count);
    [[nodiscard]] int64_t ReadNum(utf8::iterator<std::string::const_iterator>& it, int digits);
    void AdvanceToNextLine(utf8::iterator<std::string::const_iterator>& it);

    /// @brief Read until it hits the first `endChar` character.
    /// Does NOT read the `endChar` character.
    /// (i.e. After reading, `it` points to the first `endChar` character.)
    /// 
    /// @param it 
    /// @param endChar 
    /// @return 
    [[nodiscard]] std::string ReadUntil(utf8::iterator<std::string::const_iterator>& it, char32_t endChar);
}
