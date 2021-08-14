#include "ReplayRecord.hpp"

#include <format>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <utf8.h>
#include <utf8help/utf8help.hpp>

namespace rrm
{
    ReplayRecord::ReplayRecord(const SerializedString& serializedString)
    {
        // TODO: Initialize members

    }

    ReplayRecord::Version::Version(uint8_t major, uint8_t minor, uint8_t patch, uint8_t rev)
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

    uint8_t& ReplayRecord::Version::operator[](int idx)
    {
        if (idx < 0 || idx >= DIGITS)
            throw std::out_of_range(std::format("Invalid version index: {}", idx));
        return versionNums_[idx];
    }

    std::string ReplayRecord::Version::ToString() const
    {
        return std::format("{}.{}.{}.{}", versionNums_[0], versionNums_[1], versionNums_[2], versionNums_[3]);
    }

    ReplayRecord::ElementSerializedString ReplayRecord::Version::Serialize() const
    {
        return ElementSerializedString(std::format("{}{}{:0>2}{:0>2}", versionNums_[0], versionNums_[1], versionNums_[2], versionNums_[3]));
    }

    ReplayRecord::DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
        : year_(year), month_(month), day_(day), hour_(hour), minute_(minute), second_(second)
    {

    }

    std::string ReplayRecord::DateTime::ToString() const
    {
        return std::format("{}-{}-{} {}:{}:{}", year_, month_, day_, hour_, minute_, second_);
    }

    ReplayRecord::ElementSerializedString ReplayRecord::DateTime::Serialize() const
    {
        return ElementSerializedString(std::format("{:0>2}{:0>2}{:0>2}{:0>2}{:0>2}{:0>4}", hour_, minute_, second_, day_, month_, year_));
    }

    ReplayRecord::ElementSerializedString::ElementSerializedString(const std::string& str)
        : str_(str)
    {

    }

    ReplayRecord::ElementSerializedString::ElementSerializedString(std::string&& str)
        : str_(std::move(str))
    {
    }

    std::string ReplayRecord::ElementSerializedString::ToString() const
    {
        return str_;
    }

    ReplayRecord::Name::Name(const std::string& name)
    {
        Set(name);
    }

    void ReplayRecord::Name::Set(const std::string& name)
    {
        std::string tempName = name;
        utf8help::RTrim(tempName);
        tempName = utf8help::Upper(tempName);

        const int len = static_cast<int>(utf8::distance(tempName.cbegin(), tempName.cend()));
        if (len > MAX_LEN)
            throw std::length_error(std::format("Replay name too long: {} (Max {})", len, MAX_LEN));
        name_ = tempName;
        len_ = len;
    }

    std::string ReplayRecord::Name::ToString() const
    {
        return name_;
    }

    ReplayRecord::ElementSerializedString ReplayRecord::Name::Serialize() const
    {
        const std::string spaceSuffix(MAX_LEN - len_, ' ');
        return ElementSerializedString(name_ + spaceSuffix);
    }

    ReplayRecord::Description::Description(const std::string& description)
    {
        Set(description);
    }

    void ReplayRecord::Description::Set(const std::string& description)
    {
        std::string tempDesc = description;
        utf8help::RTrim(tempDesc);
        tempDesc = utf8help::Upper(tempDesc);

        const int len = static_cast<int>(utf8::distance(tempDesc.cbegin(), tempDesc.cend()));
        if (len > MAX_LEN)
            throw std::length_error(std::format("Replay name too long: {} (Max {})", len, MAX_LEN));
        description_ = tempDesc;
        len_ = len;
    }

    std::string ReplayRecord::Description::ToString() const
    {
        return description_;
    }

    ReplayRecord::ElementSerializedString ReplayRecord::Description::Serialize() const
    {
        const std::string spaceSuffix(MAX_LEN - len_, ' ');
        return ElementSerializedString(description_ + spaceSuffix);
    }

    ReplayRecord::SerializedString::SerializedString(const std::string& str)
        : str_(str)
    {

    }

    ReplayRecord::SerializedString::SerializedString(std::string&& str)
        : str_(std::move(str))
    {
    }

    std::string ReplayRecord::SerializedString::ToString() const
    {
        return str_;
    }

    ReplayRecord::GameLength::GameLength(int frames)
    {
        if (frames < MIN_LEN)
            throw std::invalid_argument(std::format("GameLength can't be negative: {}", frames));
        if (frames > MAX_LEN)
            throw std::invalid_argument(std::format("GameLength can't be bigger than {}: {}", MAX_LEN, frames));
        frames_ = frames;
    }

    int ReplayRecord::GameLength::GetFrames() const
    {
        return frames_;
    }

    std::string ReplayRecord::GameLength::ToString() const
    {
        int seconds = frames_ / 60;
        const int minutes = seconds / 60;
        seconds %= 60;
        return std::format("{}:{:0>2}", minutes, seconds);
    }

    ReplayRecord::ElementSerializedString ReplayRecord::GameLength::Serialize() const
    {
        return ElementSerializedString(std::format("{:0>6}", frames_));
    }

    std::string ReplayRecord::MatchType::ToString() const
    {
        if (isTeamMatch_ && type_ == Type::LOCAL)
            return "Local Team";
        switch (type_)
        {
        case Type::LOCAL:
            return "Local";
        case Type::ONLINE_CASUAL:
            return "Online Casual";
        case Type::FRIENDLY:
            return "Friendly";
        case Type::RANKED:
            return "Ranked";
        default:
            throw std::out_of_range(std::format("Match Type out of range of [0, 4]: {}", static_cast<int>(type_)));
        }
        return "";
    }

    ReplayRecord::ElementSerializedString ReplayRecord::MatchType::SerializeType() const
    {
        return ElementSerializedString(std::to_string(static_cast<int>(type_)));
    }

    ReplayRecord::ElementSerializedString ReplayRecord::MatchType::SerializeIsTeamMatch() const
    {
        return ElementSerializedString(isTeamMatch_ ? "1" : "0");
    }
}
