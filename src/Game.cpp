#include "Game.hpp"
#include "SDL2/SDL.h"
#include "Window.hpp"
#include "Graphics.hpp"
#include <iostream>
#include<memory>
#include "ClassicUnhoverableButtonElement.hpp"
#include "ScaleElement.hpp"
#include "BasicGameObjectHandler.hpp"
#include "ExplainableSetElement.hpp"
#include <numeric>
#include "CompositeDrawing.hpp"
#include "GameObjectFactory.hpp"

using ObjectData::ScaleValue;
using ErrorHandling::checkForErrorStandardHierarchy;
using ObjectData::PlayerID;

Game::Game(): m_gameDataGiver(*this)
{
    m_init();
}

Game::~Game()
{
    //m_mainWindow.reset(0);
    m_theFont.reset(0);
    TTF_Quit();
    SDL_Quit();
}

void Game::m_init(){
    int initCode = SDL_Init(SDL_INIT_EVERYTHING);
    if(initCode!=0){
        std::cout<<"Init fail: "<<SDL_GetError()<<std::endl;
    }
    initCode = TTF_Init();
    if(initCode!=0){
        std::cout<<"TTF_Init fail: "<<SDL_GetError()<<std::endl;
    }    
    checkForError(initCode, m_gameData.error);
    const std::string windowName = "Test";
    const std::string fontFilename = {"build/Fonts/FT88-Regular.ttf"};
    m_theFont = std::make_unique<Graphics::Font>(fontFilename);
    Graphics::theFont = m_theFont.get();

    //m_mainWindow = std::make_unique<Window>(windowName);
    //m_mainWindow->m_init();
    checkForError(m_initPlayers(), m_gameData.error);
    if(m_gameData.error != ErrorCode::OK){
        std::cout<<"Init players fail: "<<toString(m_gameData.error)<<std::endl;
    }
    checkForError(m_initEventHandler(), m_gameData.error);
    if(m_gameData.error != ErrorCode::OK){
        std::cout<<"Init event handler fail: "<<toString(m_gameData.error)<<std::endl;
    }
    checkForError(m_initPlayerUI(), m_gameData.error);
    if(m_gameData.error != ErrorCode::OK){
        std::cout<<"Init player UI fail: "<<toString(m_gameData.error)<<std::endl;
    }
}

ErrorCode Game::m_simpleTest(){
    std::vector<ObjectData::ScaleData> suggestions;
    for(int i = 0; i < 3; ++i){
        std::string aboveString = "Above"+std::to_string(i+1);
        std::string belowString = "Below"+std::to_string(i+1);
        suggestions.push_back(ObjectData::ScaleData{aboveString, belowString, ScaleValue::SCALE_1});
        suggestions.push_back(ObjectData::ScaleData{aboveString, belowString, ScaleValue::SCALE_COMMIT});
    }
    for(const auto& suggestion : suggestions){
        m_suggestionQueue.push_front(std::make_unique<Suggestor::Suggestion>(Suggestor::Suggestion::None()));
        m_suggestionQueue.front()->scaleData = std::make_unique<ObjectData::ScaleData>(suggestion);
        m_suggestionQueue.front()->applicability = Suggestor::Suggestion::ApplicabilityType::ONCE;
        m_suggestionQueue.front()->radicality = Suggestor::Suggestion::Radicality::PARTIAL_CHANGE_ONLY;
        m_suggestionQueue.front()->playerID = PlayerID::PLAYER_1;
    }
    m_suggestionQueue.push_front(std::make_unique<Suggestor::Suggestion>(Suggestor::Suggestion::None()));
    m_suggestionQueue.front()->applicability = Suggestor::Suggestion::ApplicabilityType::ONCE;
    m_suggestionQueue.front()->radicality = Suggestor::Suggestion::Radicality::PARTIAL_CHANGE_ONLY;
    m_suggestionQueue.front()->playerID = PlayerID::PLAYER_2;
    m_suggestionQueue.front()->selectedWordValue = std::make_unique<ObjectData::ExplainableSetValue>(ObjectData::ExplainableSetValue::EXSET_13);
     m_suggestionQueue.push_front(std::make_unique<Suggestor::Suggestion>(Suggestor::Suggestion::None()));
    m_suggestionQueue.front()->applicability = Suggestor::Suggestion::ApplicabilityType::ONCE;
    m_suggestionQueue.front()->radicality = Suggestor::Suggestion::Radicality::PARTIAL_CHANGE_ONLY;
    m_suggestionQueue.front()->playerID = PlayerID::PLAYER_2;
    m_suggestionQueue.front()->selectedWordValue = std::make_unique<ObjectData::ExplainableSetValue>(ObjectData::ExplainableSetValue::EXSET_14);

    try{
    ErrorCode error = ErrorCode::OK;
    //     Scale scale("above", "below", ScaleValue::SCALE_1);
    //     ScaleElement element(0, 100, &scale);
    //     std::vector<int> words(24);
    //     std::iota(words.begin(), words.end(), 0);

    //     std::vector<std::string> wordStrings;
    //     for(int i(0); i < words.size(); ++i){
    //         wordStrings.push_back(std::to_string(words[i]));
    //     }
    //     ExplainableSet explainables(wordStrings, ObjectData::ExplainableSetValue::EXSET_1);
    //     ExplainableSetElement exel(500, 500, &explainables);
    //     Drawing drawing(m_mainWindow->m_Renderer());
    //     drawing.m_add(&element);
    //     drawing.m_add(&exel);
    //     BasicGameObjectHandler e(&drawing);
    //     return e.m_eventLoop();
    while(!m_gameData.lastRoundReached){
        if(m_currentRound){
            throw std::runtime_error("Current round exists when it should not.");
            //checkForError(m_killRound(), m_gameData.error);
        }
        checkForError(m_initRound(), m_gameData.error);
        std::map<PlayerID, std::vector<GameObjectFactory::Recipe>> recipeContainer;
        checkForError(m_currentRound->m_createRoundElementRecipes(recipeContainer), m_gameData.error);
        for(auto& pair : recipeContainer){
            for(auto& recipe : pair.second){
                auto player = m_players.at(pair.first);
                checkForError(player->m_addRecipe(&recipe), m_gameData.error);
                //Confirm readiness of scales and explainable sets associated with the recipe.
            }
        }
        checkForError(m_currentRound->m_confirmReadiness(), m_gameData.error);
        checkForError(m_playRound(), m_gameData.error);
        if(m_gameData.error != ErrorCode::OK){
            std::cout<<"Play round fail: "<<toString(m_gameData.error)<<std::endl;
            break;
        }
            checkForError(m_killRound(), m_gameData.error);
    }

    ErrorCode killError = m_killGame();
    if(killError != ErrorCode::OK){
        std::cout<<"Kill game fail: "<<toString(killError)<<std::endl;
    }
    if(m_gameData.error != ErrorCode::OK)
        return m_gameData.error;
    else return killError;
    }
    catch(SDLQuitException& e){
        return e.m_error;
    }

}

ErrorCode Game::m_playRound(){
    if(!m_currentRound){
        return ErrorCode::NO_CURRENT_ROUND;
    }
    ErrorCode error = ErrorCode::OK;
    Continuation finished = CONTINUE;
    if(!m_eventHandler)
        return ErrorCode::EVENT_HANDLER_NULL;
     while(finished == CONTINUE){
        const Suggestor::Suggestion* sugg = m_currentSuggestion();
        checkForErrorStandardHierarchy(m_gameData.error, error);
        if(!sugg){//No suggestion made. This should normally not happen.
            continue;
        }
        ErrorCode validationRaw = m_currentRound->m_validate(*sugg);
        ErrorCode validation = m_interpretValidationError(validationRaw);
        if(validation == ErrorCode::OK){
            checkForErrorStandardHierarchy(m_handleValidSuggestion(*sugg), error);
        } else if(validation == ErrorCode::IGNORE_THIS){
            continue;
        } else {
            throw std::runtime_error("Unhandled validation error");
        }
        if(m_currentRound->m_isRoundFinished()){
            finished = FINISH_ROUND;
        }
        if(m_isGameFinished()){
            finished = FINISH_GAME;
        }
    }
    if(finished == FINISH_GAME){
        m_gameData.lastRoundReached = true;
    }
    return ErrorCode::OK;
}

ErrorCode Game::m_initRound(){
    ErrorCode error = ErrorCode::OK;
    if(m_currentRound){
        return ErrorCode::OLD_ROUND_NOT_KILLED;
    }
    auto playerIDs = m_playerIDs();
    m_currentRound = std::make_unique<Round>(m_gameData.roundHistory.size() + 1, m_currentPlayerID(), playerIDs, 3, 24);
    for(auto& player: m_playerList){
        checkForError(player->m_prepareForNewRound(), error);
    }
    checkForError(m_currentRound->m_initRound(), error);
    checkForError(m_eventHandler->m_updateDisplaying(), error);
    return error;
}

ErrorCode Game::m_killRound(){
    ErrorCode error = ErrorCode::OK;
    if(!m_currentRound){
        return ErrorCode::NO_CURRENT_ROUND;
    }
    std::unique_ptr<Round::RoundData> dumpData;
    checkForError(m_currentRound->m_shutdown(&m_players, dumpData), error); 
    m_gameData.roundHistory.push_back(std::move(dumpData));
    while(!m_currentRound->m_suggestionsLeftToHandle.empty()){
        m_suggestionQueue.push_back(std::move(m_currentRound->m_suggestionsLeftToHandle.back()));
        m_currentRound->m_suggestionsLeftToHandle.pop_back();
    }
    m_currentRound.reset();
    m_eventHandler->m_updateDisplaying();
    //The round's suggestions will be migrated to the game's suggestion queue

    return ErrorCode::OK;
}

ErrorCode Game::m_initEventHandler(){
    if(m_eventHandler){
        return ErrorCode::EVENT_HANDLER_ALREADY_INITIALIZED;
    }
    CompositeDrawing* drawing = new CompositeDrawing();
    for(auto& player: m_playerList){
        drawing->m_add(player->m_Drawing(), (int) player->m_Data().playerID);
    }
    m_eventHandler = std::make_unique<BasicGameObjectHandler>(drawing);
    return ErrorCode::OK;
}

ErrorCode Game::m_initPlayers(){
    if(!m_playerList.empty()){
        return ErrorCode::PLAYERS_ALREADY_INITIALIZED;
    }
    ErrorCode error = ErrorCode::OK;
    for(int i = 1; i <= m_gameData.numberOfPlayers; ++i){//Create one player at a time
        checkForError(m_initPlayer(i), error);
    }
    return error;
}

ErrorCode Game::m_initPlayer(int index){//Note that a player (at least for now) does not get any permanent game objects. Game objects come with the rounds.
    if(index <= 0 || index > m_gameData.numberOfPlayers){
        return ErrorCode::INVALID_PLAYER_INDEX;
    }
    auto player = std::make_unique<Player>(static_cast<PlayerID> (index), m_figureOutPlayerName(static_cast<ObjectData::PlayerID>(index))); 
    m_players[player->m_Data().playerID] = player.get();
    m_playerList.push_back(std::move(player));
    return ErrorCode::OK;
}

std::string Game::m_figureOutPlayerName(ObjectData::PlayerID playerID){
    std::string base = "Player ";
    base += std::to_string(static_cast<int>(playerID));
    return base;
}

std::vector<ObjectData::PlayerID> Game::m_playerIDs(){
    std::vector<ObjectData::PlayerID> ids;
    for(auto& player: m_playerList){
        ids.push_back(player->m_Data().playerID);
    }
    return ids;
}

ErrorCode Game::m_interpretValidationError(ErrorCode error){
    switch(error){
        case ErrorCode::IGNORE_THIS: //how would this even happen?
            throw std::logic_error("Encountered IGNORE_THIS error code unexpectedly.");
        case ErrorCode::ALREADY_COMMITTED:
        case ErrorCode::INVALID_PLAYER_FOR_SCALE:
        case ErrorCode::INVALID_PLAYER_FOR_SELECTED_WORD:
        case ErrorCode::SCALES_NOT_COMMITTED:
            return ErrorCode::IGNORE_THIS;
        case ErrorCode::OK:
            return ErrorCode::OK;
        case ErrorCode::WEIRD_SCALE_DATA:
        case ErrorCode::BUTTON_ELEMENT_PRESENT:
        case ErrorCode::INVALID_SELECTED_WORD:
        case ErrorCode::INVALID_PLAYER_ID:
            return error;
        default:
            return error;
    }
}

ErrorCode Game::m_confirmRoundReadiness(){
    if(!m_currentRound){
        return ErrorCode::NO_CURRENT_ROUND;
    }
    ErrorCode error = m_currentRound->m_confirmReadiness();
    checkForError(error, m_gameData.error);
    return error;
}

ErrorCode Game::m_handleValidSuggestion(const Suggestor::Suggestion& suggestion){//Quick fix: Have all game objects update their displaying. This is somewhat ugly given that many did so earlier, but it should at least work.
    if(!m_currentRound){
        return ErrorCode::NO_CURRENT_ROUND;
    }
    ErrorCode error = m_eventHandler->m_handleValidSuggestion(suggestion);
    if(error == ErrorCode::NOTHING_HAPPENED){
        return error;
    }
    if(error == ErrorCode::MERELY_VISUAL_IMPLICATIONS){
        error = ErrorCode::OK;
    }
    checkForErrorStandardHierarchy(error, m_gameData.error);
    checkForErrorStandardHierarchy(m_currentRound->m_reactToSuggestion(suggestion, error), m_gameData.error); 
    ErrorCode updateError = m_eventHandler->m_updateDisplaying();
    return error;
}

const Suggestor::Suggestion* Game::m_currentSuggestion(){
    Suggestor::Suggestion* sugg = nullptr;
    //Reset the current suggestion before entering the event loop.
    if(!m_suggestionQueue.empty()){
        sugg = m_suggestionQueue.back().get();
        std::swap(m_tmpSuggestion, m_suggestionQueue.back());
        m_suggestionQueue.pop_back();
        return sugg;
    }
    //The round's suggestions need to be checked for too. Right now, suggestions don't have  priority. Maybe they should.
    if(m_currentRound){
        auto& roundSuggestions = m_currentRound->m_suggestionsLeftToHandle;
        if(!roundSuggestions.empty()){
            sugg = roundSuggestions.back().get();
            std::swap(m_tmpSuggestion, roundSuggestions.back());
            roundSuggestions.pop_back();
            return sugg;
        }
    }
    ErrorCode resetError = m_eventHandler->m_resetSuggestion();
    if(resetError != ErrorCode::OK && resetError != ErrorCode::NOTHING_HAPPENED){// I'm not sure if, other than for the first iteration, NOTHING_HAPPENED should occur though.
        checkForErrorStandardHierarchy(resetError, m_gameData.error);
    }
    checkForErrorStandardHierarchy(m_eventHandler->m_eventLoop(), m_gameData.error);
    return m_eventHandler->m_retrieveSuggestion();
}

ErrorCode Game::m_initPlayerUI(){
    for(auto& player : m_playerList){
        if(player){
            GameObjectFactory::Recipe recipe;
            recipe.handlingMode = GameObject::HandlingMode::DISPLAY_AND_APPLY;
            recipe.longevity = GameObject::Longevity::PERMANENT;
            recipe.gameDataGiver = &m_gameDataGiver;
            recipe.player = player.get();
            recipe.type = GameObjectFactory::GameObjectType::PLAYER_STATE_INDICATING_ELEMENT;
            player->m_addRecipe(&recipe);
        }
    }
    return ErrorCode::OK;
}

const Round::RoundData& Game::m_RoundData() const{
    if(!m_currentRound)
        throw std::runtime_error("No current round available!");
    return m_currentRound->m_RoundData();
}