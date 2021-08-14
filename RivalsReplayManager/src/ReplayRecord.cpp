#include "ReplayRecord.hpp"

#include <format>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <utf8.h>
#include <utf8help/utf8help.hpp>

namespace rrm
{
    ReplayVersion::ReplayVersion(uint8_t major, uint8_t minor, uint8_t patch, uint8_t rev)
        : versionNums_{ major,minor,patch,rev }
    {
        if (major >= 10)
            throw std::invalid_argument(std::format("Invalid major(idx 0) version: {}", major));
        if (minor >= 10)
            throw std::invalid_argument(std::format("Invalid minor(idx 1) version: {}", minor));
        if (patch >= 100)
            throw std::invalid_argument(std::format("Invalid patch(idx 2) version: {}", patch));
        if (rev >= 100)
            throw std::invalid_argument(std::format("Invalid rev(idx 3) version: {}", rev));
    }

    uint8_t& ReplayVersion::operator[](int idx)
    {
        if (idx < 0 || idx >= DIGITS)
            throw std::out_of_range(std::format("Invalid version index: {}", idx));
        return versionNums_[idx];
    }

    std::string ReplayVersion::ToString() const
    {
        return std::format("{}.{}.{}.{}", versionNums_[0], versionNums_[1], versionNums_[2], versionNums_[3]);
    }

    ReplayRecordElementSerializedString ReplayVersion::Serialize() const
    {
        return std::format("{}{}{:0>2}{:0>2}", versionNums_[0], versionNums_[1], versionNums_[2], versionNums_[3]);
    }

    ReplayDateTime::ReplayDateTime(int year, int month, int day, int hour, int minute, int second)
        : year_(year), month_(month), day_(day), hour_(hour), minute_(minute), second_(second)
    {

    }

    std::string ReplayDateTime::ToString() const
    {
        return std::format("{}-{}-{} {}:{}:{}", year_, month_, day_, hour_, minute_, second_);
    }

    ReplayRecordElementSerializedString ReplayDateTime::Serialize() const
    {
        return std::format("{:0>2}{:0>2}{:0>2}{:0>2}{:0>2}{:0>4}", hour_, minute_, second_, day_, month_, year_);
    }

    ReplayRecordElementSerializedString::ReplayRecordElementSerializedString(const std::string& str)
        : str_(str)
    {

    }

    std::string ReplayRecordElementSerializedString::ToString() const
    {
        return str_;
    }

    ReplayName::ReplayName(const std::string& name)
    {
        Set(name);
    }

    void ReplayName::Set(const std::string& name)
    {
        std::string tempName = name;
        utf8help::RTrim(tempName);

        // to upper (NOT UTF8)
        // std::transform(tempName.begin(), tempName.end(), tempName.begin(), std::toupper);

        len_ = static_cast<int>(utf8::distance(name.cbegin(), name.cend()));
        if (len_ > MAX_LEN)
            throw std::length_error(std::format("Replay name too long: {} (Max {})", len_, MAX_LEN));
        name_ = name;
    }

    std::string ReplayName::ToString() const
    {
        return name_;
    }

    ReplayRecordElementSerializedString ReplayName::Serialize() const
    {
        return std::string();
    }
}
