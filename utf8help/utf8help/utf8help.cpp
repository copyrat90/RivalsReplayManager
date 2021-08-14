#include "utf8help.hpp"

#include <array>
#include <utf8.h>

namespace utf8help
{
    namespace
    {
        constexpr std::array<char32_t, 26> UPPER_LETTERS = { 'A', 'B', 'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
        constexpr std::array<char32_t, 26> LOWER_LETTERS = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
    }

    void RTrim(std::string& str)
    {
        str.erase(std::find_if(str.rbegin(), str.rend(), [](char ch) {return !std::isspace(ch); }).base(), str.end());
    }

    std::string Transform(const std::string& str, std::function<char32_t(char32_t)> func)
    {
        std::string resultStr;
        utf8::iterator it(str.cbegin(), str.cbegin(), str.cend());
        utf8::iterator endIt(str.cend(), str.cbegin(), str.cend());
        do
        {
            char32_t ch = *it;
            ch = func(ch);
            utf8::append(ch, resultStr);
        } while (++it != endIt);

        return resultStr;
    }

    std::string Upper(const std::string& str)
    {
        return Transform(str, [](char32_t ch) {
            if (LOWER_LETTERS.front() <= ch && ch <= LOWER_LETTERS.back())
            {
                int idx = ch - LOWER_LETTERS.front();
                ch = UPPER_LETTERS[idx];
            }
            return ch;
        });
    }
}