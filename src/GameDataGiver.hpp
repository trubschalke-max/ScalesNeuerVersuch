#ifndef GAMEDATAGIVER_H
#define GAMEDATAGIVER_H
#pragma once

#include "ObjectData.hpp"
#include "Round.hpp"
#include "GameData.hpp"

class Game;
struct GameData;


class GameDataGiver//This class shall provide access to the internal data of the Game class in a controlled manner.
{
private:
    const Game& m_game;
    const Round::RoundData* m_currentRoundDataCache;
public:
    GameDataGiver(const Game& game);
    ~GameDataGiver();
    const GameData& m_GameData() const;
    std::string m_PlayerNameFor(ObjectData::PlayerID playerID) const;
    const Round::RoundData* m_CurrentRoundData() const;
    const Round::RoundData* m_retrieveCurrentRoundData();
};

#endif