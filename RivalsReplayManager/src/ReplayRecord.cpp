#include "ReplayRecord.hpp"

#include <stdexcept>
#include <format>
#include <utf8help/utf8help.hpp>

namespace rrm
{
    namespace
    {
        [[nodiscard]] bool CheckWorkshopLine(utf8::iterator<std::string::const_iterator> it)
        {
            do
            {
                if ((char32_t)*it == '$')
                    return true;
            } while (((char32_t)*it++) == '\n');
            return false;
        }

        [[nodiscard]] ReplayRecord::WorkshopItem ReadWorkshopLine(utf8::iterator<std::string::const_iterator>& it)
        {
            using namespace utf8help;
            ++it; // ignore the first '1'
            const uint64_t steamId = std::stoll(ReadUntil(it, '$'));
            ++it; // ignore '$'
            const int majorVer = (int)ReadNum(it, 3);
            const int minorVer = (int)ReadNum(it, 3);
            AdvanceToNextLine(it);

            return { steamId, {majorVer, minorVer} };
        }

        [[nodiscard]] bool CheckPlayerLine(utf8::iterator<std::string::const_iterator> it)
        {
            const char32_t ch = (char32_t)*it;
            return ch == 'H' || ('1' <= ch && ch <= '9');
        }
    }

    ReplayRecord::ReplayRecord(const std::string& serializedStr)
    {
        using namespace utf8help;

        utf8::iterator it(serializedStr.cbegin(), serializedStr.cbegin(), serializedStr.cend());

        // Line 1
        starred_ = ReadNum(it, 1) == 1;

        version_.digits[0] = (uint8_t)ReadNum(it, 1);
        version_.digits[1] = (uint8_t)ReadNum(it, 1);
        version_.digits[2] = (uint8_t)ReadNum(it, 2);
        version_.digits[3] = (uint8_t)ReadNum(it, 2);

        dateTime_.hour = (int)ReadNum(it, 2);
        dateTime_.minute = (int)ReadNum(it, 2);
        dateTime_.second = (int)ReadNum(it, 2);
        dateTime_.day = (int)ReadNum(it, 2);
        dateTime_.month = (int)ReadNum(it, 2);
        dateTime_.year = (int)ReadNum(it, 4);

        name_ = RTrimSpace(ReadString(it, 32));
        description_ = RTrimSpace(ReadString(it, 140));

        unknown_3_digits_ = ReadString(it, 3);
        gameLengthInFrames_ = (int)ReadNum(it, 6);
        matchType_ = (MatchType)ReadNum(it, 1);
        unknown_10_digits_ = ReadString(it, 10);

        AdvanceToNextLine(it);

        // Line 2
        aether_ = ReadNum(it, 1) == 1;
        stage_ = (Stage)ReadNum(it, 2);
        stocks_ = (int)ReadNum(it, 2);
        timer_ = (int)ReadNum(it, 2);
        knockbackScale_ = (int)ReadNum(it, 1);
        team_ = ReadNum(it, 1) == 1;
        teamAttack_ = ReadNum(it, 1) == 1;
        showScoresOnTop_ = ReadNum(it, 1) == 1;
        turbo_ = ReadNum(it, 1) == 1;
        devMode_ = ReadNum(it, 1) == 1;
        abyssEndlessNums_ = (int)ReadNum(it, 4);
        unknown_9_digits_ = ReadString(it, 9);

        AdvanceToNextLine(it);

        // Line
        if (CheckWorkshopLine(it))
        {
            workshopStage_ = ReadWorkshopLine(it);
        }

        // TODO: Read players and footer
        // Lines
        while (CheckPlayerLine(it))
        {
            players_.push_back(Player());
            Player& player = players_.back();

            // Line
            std::string humanOrCpuLevel = ReadString(it, 1);
            if (humanOrCpuLevel == "H")
                player.cpuLevel = -1;
            else
                player.cpuLevel = std::stoi(humanOrCpuLevel);

            player.name = RTrimSpace(ReadString(it, 32));
            player.tag = RTrimSpace(ReadString(it, 6));
            player.unknown_1_digit = ReadString(it, 1);
            player.rival = (Player::Rival)ReadNum(it, 2);
            player.colorId = (int)ReadNum(it, 2);
            player.customColorId = (int)ReadNum(it, 2);
            player.redTeam = ReadNum(it, 1) == 1;
            player.unknown_7_digits = ReadString(it, 7);
            player.colorCode = RTrimSpace(ReadString(it, 50));
            player.unknown_2_digits = ReadString(it, 2);
            player.buddy = (Player::Buddy)ReadNum(it, 2);
            player.useWorkshopSkin = ReadNum(it, 1) == 1;
            player.abyssRunes = std::bitset<15>(std::stoull(ReadString(it, 15), nullptr, 2));
            player.unknown_1_digit_2 = ReadString(it, 1);
            player.score = (int)ReadNum(it, 2);
            player.unknown_8_digits = ReadString(it, 8);

            AdvanceToNextLine(it);

            // Line
            if (player.rival >= Player::Rival::RIVAL_TOTAL_COUNT)
            {
                if (!CheckWorkshopLine(it))
                    throw std::invalid_argument(std::format("Player {} has a workshop rival id of {}, but doesn't have a corresponding steam workshop line.", players_.size(), (int)player.rival));

                player.workshopRival = ReadWorkshopLine(it);
            }
            // Line
            if (player.buddy >= Player::Buddy::BUDDY_TOTAL_COUNT)
            {
                if (!CheckWorkshopLine(it))
                    throw std::invalid_argument(std::format("Player {} has a workshop buddy id of {}, but doesn't have a corresponding steam workshop line.", players_.size(), (int)player.buddy));

                player.workshopBuddy = ReadWorkshopLine(it);
            }
            // Line
            if (player.useWorkshopSkin)
            {
                if (!CheckWorkshopLine(it))
                    throw std::invalid_argument(std::format("Player {} uses a workshop skin, but doesn't have a corresponding steam workshop line.", players_.size()));

                player.workshopSkin = ReadWorkshopLine(it);
            }

            // Line
            player.moveInstructions = ReadUntil(it, '\r');
            AdvanceToNextLine(it);
        }

        unknownFooter_ = std::string(it.base(), serializedStr.end());
    }
}
