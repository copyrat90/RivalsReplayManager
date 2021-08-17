#include "ReplayRecord.hpp"

#include <stdexcept>
#include <fmt/core.h>
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
            } while (((char32_t)*it++) != '\n');
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
        abyss_ = (Abyss)ReadNum(it, 1);
        abyssEndlessNums_ = (int)ReadNum(it, 4);
        unknown_9_digits_ = ReadString(it, 9);

        AdvanceToNextLine(it);

        // Line
        if (CheckWorkshopLine(it))
        {
            workshopStage_ = ReadWorkshopLine(it);
        }

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
                    throw std::invalid_argument(fmt::format("Player {} has a workshop rival id of {}, but doesn't have a corresponding steam workshop line.", players_.size(), (int)player.rival));

                player.workshopRival = ReadWorkshopLine(it);
            }
            // Line
            if (player.buddy >= Player::Buddy::BUDDY_TOTAL_COUNT)
            {
                if (!CheckWorkshopLine(it))
                    throw std::invalid_argument(fmt::format("Player {} has a workshop buddy id of {}, but doesn't have a corresponding steam workshop line.", players_.size(), (int)player.buddy));

                player.workshopBuddy = ReadWorkshopLine(it);
            }
            // Line
            if (player.useWorkshopSkin)
            {
                if (!CheckWorkshopLine(it))
                    throw std::invalid_argument(fmt::format("Player {} uses a workshop skin, but doesn't have a corresponding steam workshop line.", players_.size()));

                player.workshopSkin = ReadWorkshopLine(it);
            }

            // Line
            player.moveInstructions = ReadUntil(it, '\r');
            AdvanceToNextLine(it);
        }

        unknownFooter_ = std::string(it.base(), serializedStr.end());

        // {Name(32) + Description(140)} * {latin(1 byte) -> other(4 bytes)}
        serializeStrMaxSize_ = (int)serializedStr.size() + (32 + 140) * 3;
    }

    std::string ReplayRecord::Serialize()
    {
        std::string result;
        result.reserve(serializeStrMaxSize_);

        // Line 1
        result += std::to_string(starred_);
        result += fmt::format("{}{}{:0>2}{:0>2}", version_.digits[0], version_.digits[1], version_.digits[2], version_.digits[3]);
        result += fmt::format("{:0>2}{:0>2}{:0>2}{:0>2}{:0>2}{:0>4}", dateTime_.hour, dateTime_.minute, dateTime_.second, dateTime_.day, dateTime_.month, dateTime_.year);
        result += name_;
        result += std::string(32 - utf8::distance(name_.begin(), name_.end()), ' ');
        result += description_;
        result += std::string(140 - utf8::distance(description_.begin(), description_.end()), ' ');
        result += unknown_3_digits_;
        result += fmt::format("{:0>6}", gameLengthInFrames_);
        result += std::to_string((int)matchType_);
        result += unknown_10_digits_;
        result += "\r\n";

        // Line 2
        result += std::to_string(aether_);
        result += fmt::format("{:0>2}", (int)stage_);
        result += fmt::format("{:0>2}", stocks_);
        result += fmt::format("{:0>2}", timer_);
        result += std::to_string(knockbackScale_);
        result += std::to_string(team_);
        result += std::to_string(teamAttack_);
        result += std::to_string(showScoresOnTop_);
        result += std::to_string(turbo_);
        result += std::to_string(devMode_);
        result += std::to_string((int)abyss_);
        result += fmt::format("{:0>4}", abyssEndlessNums_);
        result += unknown_9_digits_;
        result += "\r\n";

        // Line
        if (workshopStage_)
            result += fmt::format("1{}${: >3}{: >3}\r\n", workshopStage_->steamId, workshopStage_->versionDigits[0], workshopStage_->versionDigits[1]);

        // Lines
        for (const auto& player : players_)
        {
            // Line
            result += (player.cpuLevel == -1) ? "H" : std::to_string(player.cpuLevel);
            result += player.name;
            result += std::string(32 - utf8::distance(player.name.begin(), player.name.end()), ' ');
            result += player.tag;
            result += std::string(6 - utf8::distance(player.tag.begin(), player.tag.end()), ' ');
            result += player.unknown_1_digit;
            result += fmt::format("{:0>2}", (int)player.rival);
            result += fmt::format("{:0>2}{:0>2}", player.colorId, player.customColorId);
            result += std::to_string(player.redTeam);
            result += player.unknown_7_digits;
            result += player.colorCode;
            result += std::string(50 - utf8::distance(player.colorCode.begin(), player.colorCode.end()), ' ');
            result += player.unknown_2_digits;
            result += fmt::format("{:0>2}", (int)player.buddy);
            result += std::to_string(player.useWorkshopSkin);
            result += player.abyssRunes.to_string();
            result += player.unknown_1_digit_2;
            result += fmt::format("{: >2}", player.score);
            result += player.unknown_8_digits;
            result += "\r\n";

            // Line
            if (player.workshopRival)
                result += fmt::format("1{}${: >3}{: >3}\r\n", player.workshopRival->steamId, player.workshopRival->versionDigits[0], player.workshopRival->versionDigits[1]);
            // Line
            if (player.workshopBuddy)
                result += fmt::format("1{}${: >3}{: >3}\r\n", player.workshopBuddy->steamId, player.workshopBuddy->versionDigits[0], player.workshopBuddy->versionDigits[1]);
            // Line
            if (player.workshopSkin)
                result += fmt::format("1{}${: >3}{: >3}\r\n", player.workshopSkin->steamId, player.workshopSkin->versionDigits[0], player.workshopSkin->versionDigits[1]);

            // Line
            result += player.moveInstructions;
            result += "\r\n";
        }

        result += unknownFooter_;

        return result;
    }
}
