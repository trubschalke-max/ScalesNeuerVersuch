#include "GameDataGiver.hpp"
#include "Game.hpp"

GameDataGiver::GameDataGiver(const Game& game)
    : m_game(game)
{

}

GameDataGiver::~GameDataGiver()
{

}

const GameData& GameDataGiver::m_GameData() const
{
    return m_game.m_gameData;
}

std::string GameDataGiver::m_PlayerNameFor(ObjectData::PlayerID playerID) const
{
try{

    return m_game.m_players.at(playerID)->m_Data().playerName;
}
catch(const std::out_of_range& e){
    return "Player not found for playerID " + std::to_string(static_cast<int>(playerID));
}
}

const Round::RoundData* GameDataGiver::m_CurrentRoundData() const
{
    if(!m_currentRoundDataCache){
        if(!m_game.m_currentRound)
            return nullptr;
        return &m_game.m_currentRound->m_RoundData();
    }
        return &m_game.m_currentRound->m_RoundData();
    return m_currentRoundDataCache;
}   

const Round::RoundData* GameDataGiver::m_retrieveCurrentRoundData(){//This also caches the current round data
    if(!m_game.m_currentRound){
        if(!m_game.m_gameData.roundHistory.empty()){
            m_currentRoundDataCache = m_game.m_gameData.roundHistory.back().get();
            return m_currentRoundDataCache;
        }
        return m_currentRoundDataCache;
    }
    //In case there is a current round, cache its data and return the data
    m_currentRoundDataCache = &m_game.m_RoundData();
    if(m_currentRoundDataCache)
       return m_currentRoundDataCache;
    if(!m_game.m_gameData.roundHistory.empty())
        return m_game.m_gameData.roundHistory.back().get();
    return nullptr;
}

