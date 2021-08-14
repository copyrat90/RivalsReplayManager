#pragma once

#include <string_view>
#include <cstdint>
#include <string>

namespace rrm
{
    class ReplayRecordElementSerializedString
    {
    public:
        ReplayRecordElementSerializedString(const std::string& str);

        std::string ToString() const;

    private:
        std::string str_;
    };

    class ReplayVersion
    {
    public:
        static constexpr int DIGITS = 4;

        ReplayVersion(uint8_t major, uint8_t minor, uint8_t patch, uint8_t rev);

        uint8_t& operator[] (int idx);

        std::string ToString() const;
        ReplayRecordElementSerializedString Serialize() const;

    private:
        uint8_t versionNums_[DIGITS];
    };

    class ReplayDateTime
    {
    public:
        ReplayDateTime(int year, int month, int day, int hour, int minute, int second);

        std::string ToString() const;
        ReplayRecordElementSerializedString Serialize() const;

    private:
        int year_, month_, day_;
        int hour_, minute_, second_;
    };

    class ReplayName
    {
    public:
        static constexpr int MAX_LEN = 140;

        ReplayName(const std::string& name);
        void Set(const std::string& name);

        std::string ToString() const;
        ReplayRecordElementSerializedString Serialize() const;

    private:
        std::string name_;
        int len_;
    };

    class ReplayRecord
    {
    public:
        using Version = ReplayVersion;
        using DateTime = ReplayDateTime;
        using Name = ReplayName;


    private:
        bool isStarred_;
        Version version_;
        DateTime dateTime_;
        Name name_;
    };
}