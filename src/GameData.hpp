#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <vector>
#include <memory>
#include "Round.hpp"
#include "ErrorCode.hpp"

struct GameData{
    unsigned short int numberOfPlayers;
    std::vector<std::unique_ptr<Round::RoundData>> roundHistory;
    ErrorCode error = ErrorCode::OK;
    bool lastRoundReached = false;
};

#endif // GAMEDATA_HPP