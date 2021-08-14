#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <optional>

namespace rrm
{
    class ReplayRecord
    {
    public:
        class SerializedString
        {
        public:
            explicit SerializedString(const std::string& str);
            explicit SerializedString(std::string&& str);

            std::string ToString() const;

        private:
            std::string str_;
        };

        class ElementSerializedString
        {
        public:
            explicit ElementSerializedString(const std::string& str);
            explicit ElementSerializedString(std::string&& str);

            std::string ToString() const;

        private:
            std::string str_;
        };

        class Workshop
        {
        public:

        private:
            uint64_t itemId;
        };

        class Version
        {
        public:
            static constexpr int DIGITS = 4;

            Version() : Version(0, 0, 0, 0) {}
            Version(uint8_t major, uint8_t minor, uint8_t patch, uint8_t rev);

            uint8_t& operator[] (int idx);

            std::string ToString() const;
            ElementSerializedString Serialize() const;

        private:
            uint8_t versionNums_[DIGITS];
        };

        class DateTime
        {
        public:
            DateTime() : DateTime(0, 0, 0, 0, 0, 0) {}
            DateTime(int year, int month, int day, int hour, int minute, int second);

            std::string ToString() const;
            ElementSerializedString Serialize() const;

        private:
            int year_, month_, day_;
            int hour_, minute_, second_;
        };

        class Name
        {
        public:
            static constexpr int MAX_LEN = 32;

            Name() : Name("") {}
            Name(const std::string& name);
            void Set(const std::string& name);

            std::string ToString() const;
            ElementSerializedString Serialize() const;

        private:
            std::string name_;
            int len_;
        };

        class Description
        {
        public:
            static constexpr int MAX_LEN = 140;

            Description() : Description("") {}
            Description(const std::string& description);
            void Set(const std::string& description);

            std::string ToString() const;
            ElementSerializedString Serialize() const;

        private:
            std::string description_;
            int len_;
        };

        class GameLength
        {
        public:
            static constexpr int MIN_LEN = 0;
            static constexpr int MAX_LEN = 999'999;

            GameLength() : GameLength(0) {}
            GameLength(int frames);

            int GetFrames() const;
            std::string ToString() const;
            ElementSerializedString Serialize() const;

        private:
            int frames_;
        };

        class MatchType
        {
        public:
            enum class Type
            {
                LOCAL = 0, ONLINE_CASUAL = 1,
                FRIENDLY = 2, RANKED = 3
            };

            std::string ToString() const;
            ElementSerializedString SerializeType() const;
            ElementSerializedString SerializeIsTeamMatch() const;

        private:
            Type type_;
            bool isTeamMatch_;
        };

        class Stage
        {
        public:
            enum class Type
            {
                NONE, AETHER, BASIC
            };
            enum class Kind
            {
                TREETOP_LODGE = 1,
                FIRE_CAPITAL = 2,
                AIR_ARMADA = 3,
                THE_ROCK_WALL = 4,
                MERCHANT_PORT = 5,
                BLAZING_HIDEOUT = 7,
                TOWER_OF_HEAVEN = 8,
                TEMPEST_PEAK = 9,
                FROZEN_FORTRESS = 10,
                AETHERIAL_GATES = 11,
                THE_CEO_RING = 14,
                THE_SPIRIT_TREE = 15,
                NEO_FIRE_CAPITAL = 17,
                THE_SWAMPY_ESTUARY = 18,
                THE_FOREST_FLOOR = 19,
                JULESVALE = 20,
                TROUPPLE_POND = 21,
                AETHER_HIGH = 23,
                PRIDEMOOR_KEEP = 24,
                FROZEN_GATES = 25,
                THE_TUTORIAL_GRID = 26,
                THE_ROA_RING = 27,
                DIAMOND_GROVE = 36,

                ENDLESS_ABYSS = 12,
                TETHERBALL_COURT = 29,

                UNAVAILABLE_PRACTICE_ROOM = 22,

                UNUSED_EMPTY = 0,
                UNUSED_TREETOP_LODGE = 6,
                UNUSED_GRADIENT = 13,
                UNUSED_THE_TUTORIAL_GRID_X = 16,
                UNUSED_GRAY = 28,
                UNUSED_LOCKED = 30,
                UNUSED_RANDOM = 31,
                UNUSED_FIRE_CAPITAL = 32,
                UNUSED_AIR_ARMADA = 33,
                UNUSED_THE_ROCK_WALL = 34,
                UNUSED_MERCHANT_PORT = 35,
                UNUSED_BLAZING_HIDEOUT = 37,
                UNUSED_TOWER_OF_HEAVEN = 38,
                UNUSED_TEMPEST_PEAK = 39,

                STAGE_TOTAL_COUNT
            };

        private:
            static constexpr int STAGE_TOTAL_COUNT = static_cast<int>(Kind::STAGE_TOTAL_COUNT);
            struct StageProperty
            {
                const char* name;
                bool isUsed;
                bool isAetherAvailable;
            };
            static constexpr std::array<StageProperty, STAGE_TOTAL_COUNT> LUT_STAGE_PROPERTY = {
                StageProperty {"(Unused)", false, false},
                {"Treetop Lodge", true, true},
                {"Fire Capital", true, true},
                {"Air Armada", true, true},
                {"The Rock Wall", true, true},
                {"Merchant Port", true, true},
                {"(Unused) Treetop Lodge", false, true},
                {"Blazing Hideout", true, true},
                {"Tower of Heaven", true, true},
                {"Tempest Peak", true, true},
                {"Frozen Fortress", true, true},
                {"Aetherial Gates", true, true},
                {"Endless Abyss", true, true},
                {"(Unused)", false, true},
                {"The CEO Ring", true, false},
                {"The Spirit Tree", true, true},
                {"(Unused) The Tutorial Grid X", false, false},
                {"Neo Fire Capital", true, false},
                {"The Swampy Estuary", true, false},
                {"The Forest Floor", true, true},
                {"Julesvale", true, true},
                {"Troupple Pond", true, true},
                {"(Unavailable) Practice Room", false, false},
                {"Aether High", true, false},
                {"Pridemoor Keep", true, false},
                {"Frozen Gates", true, false},
                {"The Tutorial Grid", true, false},
                {"The RoA Ring", true, false},
                {"(Unused)", false, false},
                {"Tetherball Court", true, false},
                {"(Unused) Locked", false, false},
                {"(Unused) Random", false, false},
                {"(Unused) Fire Capital", false, false},
                {"(Unused) Air Armada", false, false},
                {"(Unused) The Rock Wall", false, false},
                {"(Unused) Merchant Port", false, false},
                {"Diamond Grove", true, false},
                {"(Unused) Blazing Hideout", false, false},
                {"(Unused) Tower of Heaven", false, false},
                {"(Unused) Tempest Peak", false, false},
            };

            bool isAether_;
            Kind kind_;
        };

        class KnockbackScale
        {

        };

        enum class Abyss {};

        class Character
        {
            enum class Kind
            {
                UNUSED_0 = 0,
                UNUSED_1 = 1,

                ZETTERBURN = 2,
                ORCANE = 3,
                WRASTOR = 4,
                KRAGG = 5,
                FORSBURN = 6,
                MAYPUL = 7,
                ABSA = 8,
                ETALUS = 9,
                ORI = 10,
                RANNO = 11,
                CLAIREN = 12,
                SYLVANOS = 13,
                ELLIANA = 14,
                SHOVEL_KNIGHT = 15,

                CHAR_TOTAL_COUNT
            };
        };

        ReplayRecord(const SerializedString& serializedString);

    private:
        // Line 1
        bool starred_;
        Version version_;
        DateTime dateTime_;
        Name name_;
        Description description_;
        std::string unknown_3_digits_;
        GameLength gameLength_;
        MatchType matchType_; // also L2: team info
        std::string unknown_10_digits_;
        // Line 2
        Stage stage_; // also aether/basic
        int stocks_; // 00: infinite
        int timer_; // -1: infinite
        KnockbackScale knockbackScale_;
        bool teamAttack_;
        bool showScoresOnTop_;
        bool turbo_;
        bool devMode_;

    };
}
