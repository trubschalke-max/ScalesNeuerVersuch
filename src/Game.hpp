#ifndef GAME_H
#define GAME_H

#include "Window.hpp"
#include "Graphics.hpp"
#include <memory>
#include "ExplainableSet.hpp"
#include "ErrorCode.hpp"
#include "Scale.hpp"
#include "Round.hpp"
#include "Player.hpp"
#include "BasicGameObjectHandler.hpp"


#pragma once

class Game //The game needs for some kind of logic checker class.
{
public:
    struct GameData{
        unsigned short int numberOfPlayers;
        std::vector<Round::RoundData> roundHistory;
        ErrorCode error = ErrorCode::OK;
        bool lastRoundReached = false;
    };
    enum Continuation{CONTINUE, FINISH_ROUND, FINISH_GAME};
private:
    //std::unique_ptr<Window> m_mainWindow;
    std::unique_ptr<Graphics::Font> m_theFont;
    std::unique_ptr<Round> m_currentRound; 
    std::vector<std::unique_ptr<Player>> m_playerList; //Storing the player objects. They might go into a playermanager class, but that's hypothetical.
    std::map<ObjectData::PlayerID, Player*> m_players; //I could also use the vector directly. But this is easier due to direct access by PlayerID (and not starting an array at 0)
    std::unique_ptr<BasicGameObjectHandler> m_eventHandler;
    GameData m_gameData;
    void m_init(); //Initializes the game, including SDL, TTF, font, players, and event handler. An error code is stored in m_gameData.error if something goes wrong.
    bool m_isGameFinished(){return false;} // I do not know how that may be implemented. Right now, it will always return false.
    ErrorCode m_initPlayers();
    ErrorCode m_initPlayer(int index);
    std::string m_figureOutPlayerName(ObjectData::PlayerID playerID); //This could theoretically involve opening a dialog to ask the player for their name, or even something crazier with online interactions.
    ErrorCode m_interpretValidationError(ErrorCode error); //The idea is to determine if the handling should equate to the one of IGNORE_THIS, OK, or some other error code.


    auto& m_RoundHistory(){return m_gameData.roundHistory;}
public:
    ErrorCode m_simpleTest();
    Game();
    virtual ~Game();
    ErrorCode m_initEventHandler();
    ErrorCode m_initializeRound(); //Only possible if there is no current round. Initializes a new round.
    ErrorCode m_playRound();//This executes an already initialized round.
    ErrorCode m_killRound(); //Kills the current round and moves its data into the round history. Only possible if there is a current round.
    ObjectData::PlayerID m_currentPlayerID(){return (ObjectData::PlayerID)(m_RoundHistory().size() % m_gameData.numberOfPlayers + 1);} //This refers to the current explaining player.
    ErrorCode m_killGame(){std::cout<<"Game killed"<<std::endl; return ErrorCode::OK;} //In a complete implementation, this would handle cleanup and finalization of the game. 

    std::vector<ObjectData::PlayerID> m_playerIDs();
};

#endif