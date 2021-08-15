#include "utf8help.hpp"

#include <array>

namespace utf8help
{
    namespace
    {
        constexpr std::array<char32_t, 26> UPPER_LETTERS = { 'A', 'B', 'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
        constexpr std::array<char32_t, 26> LOWER_LETTERS = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
    }

    std::string RTrimSpace(const std::string& str)
    {
        utf8::iterator it(str.cend(), str.cbegin(), str.cend());
        const utf8::iterator beginIt(str.cbegin(), str.cbegin(), str.cend());
        --it;
        while (true)
        {
            auto ch = (char32_t)*it;
            if (ch == ' ')
            {
                if (it == beginIt)
                    break;
                --it;
            }
            else
            {
                ++it;
                break;
            }
        }
        return std::string(beginIt.base(), it.base());
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

    std::string ReadString(utf8::iterator<std::string::const_iterator>& it, int count)
    {
        std::string result;
        while (count--)
            utf8::append((char32_t)*it++, result);
        return result;
    }

    int64_t ReadNum(utf8::iterator<std::string::const_iterator>& it, int digits)
    {
        return std::stoll(ReadString(it, digits));
    }

    void AdvanceToNextLine(utf8::iterator<std::string::const_iterator>& it)
    {
        while (((char32_t)*it++) != '\n');
    }

    std::string ReadUntil(utf8::iterator<std::string::const_iterator>& it, char32_t endChar)
    {
        std::string result;
        while (true)
        {
            char32_t ch = *it;
            if (ch == endChar)
                return result;
            utf8::append(ch, result);
            ++it;
        }
        return result;
    }
}
