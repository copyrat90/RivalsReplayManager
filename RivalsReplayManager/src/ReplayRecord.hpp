#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <string>
#include <optional>
#include <bitset>

namespace rrm
{
    /// @brief Stores replay file(`*.roa`) deserialized info.
    /// Note that std::string contained in it are UTF-8 encoded, and doesn't contain any newline char.
    class ReplayRecord
    {
    public:
        struct WorkshopItem
        {
            uint64_t steamId;
            std::array<int, 2> versionDigits;
        };

        struct Version
        {
            std::array<uint8_t, 4> digits;
        };

        struct DateTime
        {
            int year, month, day;
            int hour, minute, second;
        };

        enum class MatchType
        {
            LOCAL = 0, ONLINE_CASUAL = 1,
            FRIENDLY = 2, RANKED = 3
        };

        enum class Stage
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
        static constexpr int STAGE_TOTAL_COUNT = static_cast<int>(Stage::STAGE_TOTAL_COUNT);
        struct StageProperty
        {
            const char* name;
            bool used;
            bool aetherAvailable;
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

        enum class Abyss
        {
            NONE = 0, ENDLESS = 1, VS = 2
        };

        struct Player
        {
            enum class Rival
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

                RIVAL_TOTAL_COUNT
            };
            static constexpr int RIVAL_TOTAL_COUNT = static_cast<int>(Rival::RIVAL_TOTAL_COUNT);
            static constexpr std::array<const char*, RIVAL_TOTAL_COUNT> LUT_RIVAL_NAME = {
                "(Unused)", "(Unused)",
                "Zetterburn", "Orcane", "Wrastor", "Kragg", "Forsburn",
                "Maypul", "Absa", "Etalus", "Ori", "Ranno",
                "Clairen", "Sylvanos", "Elliana", "Shovel Knight",
            };

            enum class Buddy
            {
                NONE = 0,
                FROSTBEAR_CUB = 1,
                SUNNY = 2,
                KRAGGLING = 3,
                TOMMY_PRINCKLES = 4,
                DUNDEE = 5,
                PHILLIP = 6,
                CATNIP_BRAVO = 7,
                CANNONBREED = 8,
                ALDOR = 9,
                TONY_TAILSLIDE = 10,
                FREYA = 11,
                SUNBERT = 12,
                SYLVLING = 13,
                KU = 14,
                CLAIRESSA = 15,
                MELBY = 16,
                LOXOBOT = 17,
                CAESAR = 18,

                BUDDY_TOTAL_COUNT
            };
            static constexpr int BUDDY_TOTAL_COUNT = static_cast<int>(Buddy::BUDDY_TOTAL_COUNT);
            static constexpr std::array<const char*, BUDDY_TOTAL_COUNT> LUT_BUDDY_NAME = {
               "",
               "Frostbear Cub", "Sunny", "Kraggling", "Tommy Princkles", "Dundee",
               "Phillip", "Catnip Bravo", "Cannonbreed", "Aldor", "Tony Tailslide",
               "Freya", "Sunbert", "Sylvling", "Ku", "Clairessa",
               "Melby", "Loxobot", "Caesar",
            };

            // Line 1
            int cpuLevel; // -1: Human
            std::string name;
            std::string tag;
            std::string unknown_1_digit;
            Rival rival; // if >= RIVAL_TOTAL_COUNT, treated as Workshop rival
            int colorId; // if == 41, custom color
            int customColorId; // custom 1 == 08 / custom 2 == 09 / no custom == same as colorIdx
            bool redTeam;
            std::string unknown_7_digits;
            std::string colorCode;
            std::string unknown_2_digits;
            Buddy buddy; // if >= BUDDY_TOTAL_COUNT, treated as Workshop buddy
            bool useWorkshopSkin;
            std::bitset<15> abyssRunes;
            std::string unknown_1_digit_2;
            int score;
            std::string unknown_8_digits;

            // Line
            std::optional<WorkshopItem> workshopRival;
            // Line
            std::optional<WorkshopItem> workshopBuddy;
            // Line
            std::optional<WorkshopItem> workshopSkin;

            // Line
            std::string moveInstructions;

            // This ctor exists only to suppress warnings about uninitialized member variables
            Player() : cpuLevel(0), rival(Rival::UNUSED_0), colorId(0), customColorId(0), redTeam(false), buddy(Buddy::NONE), useWorkshopSkin(false), score(0) {}
        };

    private:
        // Line 1
        bool starred_;
        Version version_;
        DateTime dateTime_;
        std::string name_;
        std::string description_;
        std::string unknown_3_digits_;
        int gameLengthInFrames_;
        MatchType matchType_;
        std::string unknown_10_digits_;

        // Line 2
        bool aether_;
        Stage stage_;
        int stocks_; // 00: infinite
        int timer_; // -1: infinite
        int knockbackScale_; // printed value: half
        bool team_;
        bool teamAttack_;
        bool showScoresOnTop_;
        bool turbo_;
        bool devMode_;
        Abyss abyss_;
        int abyssEndlessNums_;
        std::string unknown_9_digits_;

        // Line
        std::optional<WorkshopItem> workshopStage_;

        // Players (multi-line)
        std::vector<Player> players_;

        // unknown footer
        std::string unknownFooter_;

        // Max size of string returned by this->Serialize()
        int serializeStrMaxSize_;

    public:
        /// @brief Parse ReplayRecord from the `serializedStr`, which is read from the `*.roa` file and uses newline as CRLF(`\r\n`).
        /// @param serializedStr raw replay file(`*.roa`) string which contains newline as CRLF(`\r\n`)
        ReplayRecord(const std::string& serializedStr);

        /// @brief Serialize ReplayRecord to std::string, so that it can be re-written to the `*.roa` file.
        /// Uses CRLF(`\r\n`) as newline.
        /// @return Serialized string ready to be written back to the `*.roa` file
        [[nodiscard]] std::string Serialize();
    };
}
