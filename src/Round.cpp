#include "Round.hpp"
#include "ExplainableSet.hpp"
#include <algorithm>

// C++ program to generate a random number in a range

#include <iostream>
#include <random>
using namespace std;
using ObjectData::ExplainableSetValue;
 

Round::Round(PlayerID explainingPlayer, const std::vector<PlayerID>& players, unsigned short int scalesCount, unsigned short int explainablesCount, unsigned short int scaleMaxValue):    m_scalesCount(scalesCount),    m_explainablesCount(explainablesCount), m_scaleMaxValue(scaleMaxValue)
{
    m_roundData.explainingPlayer = explainingPlayer;
    m_roundData.players = players;
}

Round::~Round()
{

}

std::vector<std::string> Round::m_generateExplainableWords(){//For now, a temporary implementation will only provide an increasing sequence of numbers as words.
    std::vector<std::string> words;
    for(unsigned short int i = 0; i < m_explainablesCount; ++i){
        words.push_back("Word" + std::to_string(i+1));
    }
    return words;
}

std::vector<ObjectData::ScaleData> Round::m_generateScaleData(){//For now, a temporary implementation will only provide an increasing sequence of numbers as scale data.
    std::vector<ScaleData> scaleData;
    for(unsigned short int i = 0; i < m_scalesCount; ++i){
        ScaleData item;
        item.minValue = ScaleValue::SCALE_0;
        item.maxValue = (ScaleValue)(m_scaleMaxValue);
        item.selection = ScaleValue::SCALE_NO_VALUE;
        item.state = ObjectData::State::NOT_READY;
        item.above = "Above" + std::to_string(i+1);
        item.below = "Below" + std::to_string(i+1);
        scaleData.push_back(item);
    }
    return scaleData;
}

ErrorCode Round::m_initializeRound(){
    auto explainableWords = m_generateExplainableWords();
    auto scaleData = m_generateScaleData();
    unsigned short int correctExplainableValue = m_generateCorrectExplainableValue();
    m_roundData.explainableSet = std::make_unique<ExplainableSet>(explainableWords, (ExplainableSetValue) correctExplainableValue);
    for(unsigned short int i = 0; i < m_scalesCount; ++i){
        //Make a scale out of the scale data
        m_roundData.scales.push_back(std::make_unique<Scale>(scaleData[i]));
    }   
    return ErrorCode::OK;
}

unsigned short int Round::m_generateCorrectExplainableValue(){
    //I need a random value within the range of explainable values.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, m_explainablesCount - 1);
    return (unsigned short int)(distrib(gen));
}

ErrorCode Round::m_validate(const Suggestor::Suggestion& suggestion){
    //This is not all. But I will implement the short version: Scale selections from the explaining player, word selections from the other players. The first check will be if the suggestion is not trivially invalid, i.e. its own validate method returns an error.
    ErrorCode error = ErrorCode::OK;
    checkForError(suggestion.m_validate(), error);
    if(suggestion.playerID != PlayerID::PLAYER_NONE && std::find(m_roundData.players.begin(), m_roundData.players.end(), suggestion.playerID) == m_roundData.players.end()){
        return ErrorCode::INVALID_PLAYER_ID;
    }

    if(suggestion.scaleData){
        if(std::find_if(m_roundData.scales.begin(), m_roundData.scales.end(), [&suggestion](const std::unique_ptr<Scale>& sd){ return sd->m_Data().coincidesWith(*suggestion.scaleData, false); }) != m_roundData.scales.end()){//The scale data must belong to a scale that actually exists
            return ErrorCode::WEIRD_SCALE_DATA; //This is a little oversimplified.
        }
        if(suggestion.scaleData->state == ObjectData::State::COMMITTED){
            return ErrorCode::ALREADY_COMMITTED;
        }
        if(suggestion.playerID != m_roundData.explainingPlayer){
            return ErrorCode::INVALID_PLAYER_FOR_SCALE;
        }
    }
    if(suggestion.buttonElement){//Currently, I don't have any non-composite button elements in the game, so this should not be present.
        return ErrorCode::BUTTON_ELEMENT_PRESENT;
    }
    if(suggestion.selectedWordValue){
        if(suggestion.playerID != m_roundData.guessingPlayer){
            return ErrorCode::INVALID_PLAYER_FOR_SELECTED_WORD;
        }
        if(m_roundData.explainableSet->m_contains(*suggestion.selectedWordValue) == false){
            return ErrorCode::INVALID_SELECTED_WORD;
        }
        if(m_roundData.m_areAllScalesCommitted() == false){
            return ErrorCode::SCALES_NOT_COMMITTED;
        }
    }
    return ErrorCode::OK;
}

ErrorCode Round::m_updateGuessingPlayer(){
    if(!m_shallUpdateGuessingPlayer()){
        return ErrorCode::SHOULD_NOT_UPDATE_GUESSING_PLAYER;
    }
    if(m_roundData.guessingPlayer == PlayerID::DUMMY_PLAYER){//This happens at the start.
        m_roundData.guessingPlayer = m_firstGuessingPlayer();
    }
    m_roundData.guessingPlayer = m_firstPlayerAfter(m_roundData.guessingPlayer, m_roundData.explainingPlayer);
    return ErrorCode::OK;
}

Round::PlayerID Round::m_firstGuessingPlayer() const{
    if(m_roundData.explainingPlayer == PlayerID::DUMMY_PLAYER){
        throw std::logic_error("The explaining player has not been set.");
    }
    PlayerID firstGuessingPlayer = m_firstPlayerAfter(m_roundData.explainingPlayer, m_roundData.explainingPlayer);
    return firstGuessingPlayer;
}

bool Round::m_shallUpdateGuessingPlayer() const{
    //For now, always return true. This should be replaced with actual logic.
    return true;
}

Round::PlayerID Round::m_firstPlayerAfter(PlayerID player, PlayerID excludingPlayer) const{
    if(m_roundData.players.empty()){
        throw std::logic_error("No players available.");
    }
    auto it = std::find(m_roundData.players.begin(), m_roundData.players.end(), player);
    if(it == m_roundData.players.end()){
        throw std::logic_error("Specified player not found in the list of players.");
    }
    do{
        ++it;
        if(it == m_roundData.players.end()){
            it = m_roundData.players.begin();
        }
    }while(*it == excludingPlayer);
    return *it;
}

bool Round::RoundData::m_areAllScalesCommitted() const{
    for(const auto& scale : scales){
        if(!(scale->m_Data().state != ObjectData::State::COMMITTED)){
            return false;
        }
    }
    return true;
}

ErrorCode Round::m_createRoundElementRecipes(RecipeListing& recipes){
    ErrorCode error = ErrorCode::OK;
    if(m_roundData.players.empty()){
        return ErrorCode::NO_PLAYERS_AVAILABLE;
    }
    if(!recipes.empty()){
        return ErrorCode::PLEASE_CLEAR_RECIPES; // Ensure the recipe listing is empty before creating new recipes.
    }
    for(const auto& player : m_roundData.players){
        std::vector<Recipe> playerRecipes;
        ErrorCode ec = m_createRoundElementRecipesForPlayer(player, playerRecipes);
        checkForError(ec, error); // Check for errors using the standard error handling mechanism.
        recipes[player] = std::move(playerRecipes);
    }
    return error;
}

ErrorCode Round::m_createRoundElementRecipesForPlayer(PlayerID player, std::vector<Recipe>& recipes){
    // I need a count of ScaleElements and one ExplainableSetElement. Right now, that's it.
    ErrorCode error;
    for(auto& scaleElement : m_roundData.scales){
        error = m_addScaleElementRecipeForPlayer(player, scaleElement.get(), recipes);
        checkForError(error, error);
    }

    checkForError(m_addExplainableSetElementRecipeForPlayer(player, m_roundData.explainableSet.get(), recipes), error);
    return error;
}

ErrorCode Round::m_addScaleElementRecipeForPlayer(PlayerID player, Scale* scale, std::vector<Recipe>& recipes){
    Recipe recipe; // Create a new recipe for the scale element.
    recipe.playerID = player;
    recipe.scale = scale;
    recipe.isForExplainingPlayer = (player == m_roundData.explainingPlayer);
    recipe.type = GameObjectType::SCALE_ELEMENT;
    recipes.push_back(std::move(recipe));
    return ErrorCode::OK;
}

ErrorCode Round::m_addExplainableSetElementRecipeForPlayer(PlayerID player, ExplainableSet* explainableSet, std::vector<Recipe>& recipes){
    Recipe recipe; // Create a new recipe for the explainable set element.
    recipe.playerID = player;
    recipe.isForExplainingPlayer = (player == m_roundData.explainingPlayer);
    recipe.type = GameObjectType::EXPLAINABLE_SET_ELEMENT;
    recipe.exSet = explainableSet;
    recipes.push_back(std::move(recipe));
    return ErrorCode::OK;
}