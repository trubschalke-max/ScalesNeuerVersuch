#include "Round.hpp"
#include "ExplainableSet.hpp"
#include <algorithm>

// C++ program to generate a random number in a range

#include <iostream>
#include <random>
using namespace std;
using ObjectData::ExplainableSetValue;
#include "Graphics.hpp"
#include "Player.hpp"
 

Round::Round(int nr, PlayerID explainingPlayer, const std::vector<PlayerID>& players, unsigned short int scalesCount, unsigned short int explainablesCount, unsigned short int scaleMaxValue):    m_scalesCount(scalesCount),    m_explainablesCount(explainablesCount), m_scaleMaxValue(scaleMaxValue)
{
    m_roundData = std::make_unique<RoundData>();
    m_roundData->explainingPlayer = explainingPlayer;
    m_roundData->players = players;
    m_roundData->roundNumber = nr;
    m_roundData->guessingPlayer = m_firstGuessingPlayer();
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

ErrorCode Round::m_initRound(){
    auto explainableWords = m_generateExplainableWords();
    auto scaleData = m_generateScaleData();
    unsigned short int correctExplainableValue = m_generateCorrectExplainableValue();
    m_roundData->explainableSet = std::make_unique<ExplainableSet>(explainableWords, (ExplainableSetValue) correctExplainableValue);
    for(unsigned short int i = 0; i < m_scalesCount; ++i){
        //Make a scale out of the scale data
        m_roundData->scales.push_back(std::make_unique<Scale>(scaleData[i]));
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
    checkForError(m_sanityCheck(), error);

    checkForError(suggestion.m_validate(), error);
    if(suggestion.playerID != PlayerID::PLAYER_NONE && std::find(m_roundData->players.begin(), m_roundData->players.end(), suggestion.playerID) == m_roundData->players.end()){
        return ErrorCode::INVALID_PLAYER_ID;
    }

    if(suggestion.scaleData){
        if(std::find_if(m_roundData->scales.begin(), m_roundData->scales.end(), [&suggestion](const std::unique_ptr<Scale>& sd){ return sd->m_Data().coincidesWith(*suggestion.scaleData, false); }) == m_roundData->scales.end()){//The scale data must belong to a scale that actually exists
            return ErrorCode::WEIRD_SCALE_DATA; //This is a little oversimplified.
        }
        if(suggestion.scaleData->state == ObjectData::State::COMMITTED){
            return ErrorCode::ALREADY_COMMITTED;
        }
        if(suggestion.playerID != m_roundData->explainingPlayer){
            return ErrorCode::INVALID_PLAYER_FOR_SCALE;
        }
    }
    if(suggestion.buttonElement){//Currently, I don't have any non-composite button elements in the game, so this should not be present.
        return ErrorCode::BUTTON_ELEMENT_PRESENT;
    }
    if(suggestion.selectedWordValue){
        if(suggestion.playerID != m_roundData->guessingPlayer){
            return ErrorCode::INVALID_PLAYER_FOR_SELECTED_WORD;
        }
        if(m_roundData->explainableSet->m_contains(*suggestion.selectedWordValue) == false){
            return ErrorCode::INVALID_SELECTED_WORD;
        }
        if(m_roundData->m_areAllScalesCommitted() == false){
            return ErrorCode::SCALES_NOT_COMMITTED;
        }
    }
    return error;
}

ErrorCode Round::m_updateGuessingPlayer(){// Updates the guessing player to the next one in line, skipping the explaining player. Until now, this is too complicated of a game logic. It shall later be triggered after a guess that successfully misses the correct word.
    if(!m_shallUpdateGuessingPlayer()){
        return ErrorCode::SHOULD_NOT_UPDATE_GUESSING_PLAYER;
    }
    if(m_roundData->guessingPlayer == PlayerID::DUMMY_PLAYER){//This happens at the start.
        m_roundData->guessingPlayer = m_firstGuessingPlayer();
    }
    m_roundData->guessingPlayer = m_firstPlayerAfter(m_roundData->guessingPlayer, m_roundData->explainingPlayer);
    return ErrorCode::OK;
}

Round::PlayerID Round::m_firstGuessingPlayer() const{
    if(m_roundData->explainingPlayer == PlayerID::DUMMY_PLAYER){
        throw std::logic_error("The explaining player has not been set.");
    }
    PlayerID firstGuessingPlayer = m_firstPlayerAfter(m_roundData->explainingPlayer, m_roundData->explainingPlayer);
    return firstGuessingPlayer;
}

bool Round::m_shallUpdateGuessingPlayer() const{
    //For now, always return true. This should be replaced with actual logic.
    return true;
}

Round::PlayerID Round::m_firstPlayerAfter(PlayerID player, PlayerID excludingPlayer) const{
    if(m_roundData->players.empty()){
        throw std::logic_error("No players available.");
    }
    auto it = std::find(m_roundData->players.begin(), m_roundData->players.end(), player);
    if(it == m_roundData->players.end()){
        throw std::logic_error("Specified player not found in the list of players.");
    }
    do{
        ++it;
        if(it == m_roundData->players.end()){
            it = m_roundData->players.begin();
        }
    }while(*it == excludingPlayer);
    return *it;
}

bool Round::RoundData::m_areAllScalesCommitted() const{
    for(const auto& scale : scales){
        if((scale->m_Data().state != ObjectData::State::COMMITTED)){
            return false;
        }
    }
    return true;
}

ErrorCode Round::m_createRoundElementRecipes(RecipeListing& recipes){
    ErrorCode error = ErrorCode::OK;
    if(m_roundData->players.empty()){
        return ErrorCode::NO_PLAYERS_AVAILABLE;
    }
    if(!recipes.empty()){
        return ErrorCode::PLEASE_CLEAR_RECIPES; // Ensure the recipe listing is empty before creating new recipes.
    }
    for(const auto& player : m_roundData->players){
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
    for(auto& scaleElement : m_roundData->scales){
        error = m_addScaleElementRecipeForPlayer(player, scaleElement.get(), recipes);
        checkForError(error, error);
    }

    checkForError(m_addExplainableSetElementRecipeForPlayer(player, m_roundData->explainableSet.get(), recipes), error);
    return error;
}

ErrorCode Round::m_addScaleElementRecipeForPlayer(PlayerID player, Scale* scale, std::vector<Recipe>& recipes){
    Recipe recipe; // Create a new recipe for the scale element.
    recipe.playerID = player;
    recipe.scale = scale;
    recipe.isForExplainingPlayer = (player == m_roundData->explainingPlayer);
    recipe.type = GameObjectType::SCALE_ELEMENT;
    recipes.push_back(std::move(recipe));
    return ErrorCode::OK;
}

ErrorCode Round::m_addExplainableSetElementRecipeForPlayer(PlayerID player, ExplainableSet* explainableSet, std::vector<Recipe>& recipes){
    Recipe recipe; // Create a new recipe for the explainable set element.
    recipe.playerID = player;
    recipe.isForExplainingPlayer = (player == m_roundData->explainingPlayer);
    recipe.type = GameObjectType::EXPLAINABLE_SET_ELEMENT;
    recipe.exSet = explainableSet;
    recipes.push_back(std::move(recipe));
    return ErrorCode::OK;
}

ErrorCode Round::m_confirmReadiness(){
    for(auto& scale : m_roundData->scales){
        scale->m_setState(ObjectData::State::NOT_YET_COMMITTED);
    }
    if(!m_roundData->explainableSet){
        return ErrorCode::NO_EXPLAINABLE_SET;
    }
    return ErrorCode::OK;
}

ErrorCode Round::m_sanityCheck(){
    if(m_roundData->players.empty()){
        return ErrorCode::NO_PLAYERS_AVAILABLE;
    }
    if(!m_roundData->explainableSet){
        return ErrorCode::NO_EXPLAINABLE_SET;
    }
    if(m_roundData->scales.empty()){
        return ErrorCode::NO_SCALES_AVAILABLE;
    }
    if(m_roundData->explainingPlayer == PlayerID::DUMMY_PLAYER){
        return ErrorCode::NO_EXPLAINING_PLAYER;
    }
    if(m_roundData->guessingPlayer == PlayerID::DUMMY_PLAYER){
        return ErrorCode::NO_GUESSING_PLAYER;
    }
    if(m_roundData->explainingPlayer == m_roundData->guessingPlayer){
        return ErrorCode::EXPLAINING_AND_GUESSING_PLAYER_COINCIDE;
    }
    return ErrorCode::OK;
}

ErrorCode Round::m_reactToSuggestion(const Suggestor::Suggestion& suggestion, ErrorCode handlingResult){
    ErrorCode error = handlingResult;
    if(handlingResult == ErrorCode::NOTHING_HAPPENED){
        return ErrorCode::NOTHING_HAPPENED;
    }
    ErrorHandling::checkForErrorStandardHierarchy(suggestion.certainlyKnownError, error);
    if(handlingResult == ErrorCode::OK) {
        //After a word selection that did not hit the correct explainable, we want to assign the guessing privilege to a different player.
        if(suggestion.selectedWordValue){
            checkForError(m_reactToWordSelection(suggestion), error);
        } else if(suggestion.scaleData){
            checkForError(m_reactToScaleModification(suggestion), error);
        }
    }
    if(error != ErrorCode::OK && error != ErrorCode::NOTHING_HAPPENED){
        throw std::runtime_error("Error occurred during suggestion handling: " + std::to_string(static_cast<int>(error)));
    }
    return error;
}

bool Round::m_isRoundFinished(){
    if(!m_roundData->explainableSet){
        throw std::runtime_error("No explainable set available.");
    }
    if(m_roundData->explainableSet->m_isCorrectlyGuessed()){
        return true;
    }
    if(m_roundData->explainableSet->m_numberOfGuesses() >= m_roundData->guessesAllowed){
        return true;
    }
    return false;
}

ErrorCode Round::m_reactToWordSelection(const Suggestor::Suggestion& suggestion){
    ErrorCode error = ErrorCode::OK;
    //If the selected word value is correct, it is assumed that the game will notice via the m_isFinished() function.
    //In the future, I might assign points to players. It might even be useful to have both game and round manage a queue of suggestions, including point suggestions.
    if(!suggestion.selectedWordValue){
        throw std::logic_error("No selected word value provided in the suggestion.");
    }
    if(m_roundData->explainableSet->m_isCorrectlyGuessed()){
    error = ErrorCode::NOTHING_HAPPENED;
    }
    else{
        error = m_updateGuessingPlayer();
    }    
    return error;
}

ErrorCode Round::m_reactToScaleModification(const Suggestor::Suggestion& suggestion){
    if(!suggestion.scaleData){
        throw std::logic_error("No scale data provided in the suggestion.");
    }
    //Currently, nothing happens when a scale modification suggestion is received.
    return ErrorCode::NOTHING_HAPPENED;
}

ErrorCode Round::m_shutdown(std::map<ObjectData::PlayerID, Player*>* m_players, std::unique_ptr<RoundData>& dumpData){
ErrorCode error = ErrorCode::OK;
    if(!m_players){
        throw std::logic_error("No player map provided to shutdown.");
    }

    //Remove the game objects from the players
    for(auto& pair: *m_players){
        Player* playerPtr = pair.second;
        if(!playerPtr){
            throw std::logic_error("Player pointer is null in the player map.");
        }
        checkForError(playerPtr->m_removeGameObjects(GameObject::Longevity::ONE_ROUND_LONG), error);
    }

    RoundData tmp; // Reset the round data to its default state. It is assumed that the round data is being moved to the round history before this call. In particular, std::move has been called already.
    // Determine points before (!) resetting the round data.
    auto points = m_roundData->m_determinePoints();
    dumpData = std::move(m_roundData);
    m_roundData = std::unique_ptr<RoundData>(new RoundData());
    if(points.empty()){
        checkForError(ErrorCode::NO_POINTS_COMPUTED, error);
    }
    for(const auto& pair : points){
        ObjectData::PlayerID playerID = pair.first;
        int playerPoints = pair.second;
        if(playerPoints != 0){
            m_suggestionsLeftToHandle.push_front(std::make_unique<Suggestor::Suggestion>());
            m_suggestionsLeftToHandle.front()->pointsIncrease = playerPoints;
            m_suggestionsLeftToHandle.front()->playerID = playerID;
            m_suggestionsLeftToHandle.front()->applicability = Suggestor::Suggestion::ApplicabilityType::ONCE;
            m_suggestionsLeftToHandle.front()->radicality = Suggestor::Suggestion::Radicality::PARTIAL_CHANGE_ONLY;
        }
    }

    return error;
}

int Round::RoundData::m_numberOfGuessesMade() const{
    int guesses = 0;
    for(const auto& explainable : explainableSet->m_Explainables()){
        if((explainable.state & ObjectData::WordState::TRIED) != ObjectData::WordState::ZERO){
            ++guesses;
        }
    }
    return guesses;
}

bool Round::RoundData::m_isCorrectlyGuessed() const{
    for(const auto& explainable : explainableSet->m_Explainables()){
        if((explainable.state & ObjectData::WordState::CORRECT) != ObjectData::WordState::ZERO){
            if((explainable.state & ObjectData::WordState::TRIED) != ObjectData::WordState::ZERO){
                return true;
            }
        }
    }
    return false;
}

std::map<ObjectData::PlayerID, int> Round::RoundData::m_determinePoints(bool guesserHasGuessedAlready) const{
    std::map<ObjectData::PlayerID, int> points;
    int guessingplayerIndex;
    int playerIndex = 0;
    bool committed = m_areAllScalesCommitted();
    const int guessersCount = static_cast<int>(players.size()) - 1; // Exclude the explaining player from the guessers count.
    for(const auto& player : players){
        guessingplayerIndex = m_indexOfPlayer(player);
        points[player] = 0; 
        if(!committed)
            continue; // Skip scoring for players if not all scales are committed.
        //Question: Who guessed it wrong?
        //One point for each incorrect guess, minus three points for a correct guess. The explaining player gets the maximum amount of points minus one. But every guess gives points, so it is effectively -2 instead of +1.
        if((player == guessingPlayer || player == explainingPlayer) && m_isCorrectlyGuessed()){
            points[player] -= 3; // Minus three points for a correct guess.
        }
        if(player != explainingPlayer){
            int guessesMade = m_numberOfGuessesMade() + (guesserHasGuessedAlready ? 0 : 1);
            int pointsForPlayer = guessesMade / guessersCount + (guessesMade % guessersCount >= static_cast<int>(guessingplayerIndex) ? 1 : 0); //This counts the number of guesses the player made
            points[player] += pointsForPlayer;
        }
        if(player == explainingPlayer){
            int pointsForExplainingPlayer = m_numberOfGuessesMade() / guessersCount + 1; //The idea is that the explaining player gets one extra point on top of the average guesses made by the guessers. If the guessers hit the correct word, he's already lost three points.
            points[player] += pointsForExplainingPlayer;
        }

        ++playerIndex;

    }
    return points;
}

Round::RoundData::PlayerIndex Round::RoundData::m_indexOfPlayer(PlayerID player) const{
    //First, find the explaining player.
    if(player == explainingPlayer) return EXPLAINING_PLAYER;
    auto it = std::find(players.begin(), players.end(), explainingPlayer);
    auto it2 = std::find(players.begin(), players.end(), player);
    if(it2 == players.end()) return INVALID_PLAYER_INDEX;
    auto distance = std::distance(it, it2);
    if(distance < 0){
        return static_cast<PlayerIndex>(distance + players.size());
    }
    else // If the distance is non-negative, return it as the player index.
        return static_cast<PlayerIndex>(distance);
}