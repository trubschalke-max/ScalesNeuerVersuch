#include "PlayerStateIndicatingElement.hpp"
#include "Player.hpp"

using ObjectData::Activeness;


PlayerStateIndicatingElement::PlayerStateIndicatingElement(GameDataGiver& gameDataGiver, Player* player, int x, int y)
: HoverableCompositeButtonElement(player->m_Data().playerID, x, y), m_gameDataGiver(gameDataGiver), m_player(player)
{
    m_colors = s_standardColors();
    ErrorCode result = m_createButtons();
    if(result != ErrorCode::OK){
        throw std::runtime_error("Unable to create buttons for player state indicating element!");
    }
    m_displayingSupporter = std::make_unique<PlayerStateIndicatingElementDisplayingSupporter>(this);
}

PlayerStateIndicatingElement::~PlayerStateIndicatingElement()
{

}

ErrorCode PlayerStateIndicatingElement::m_refreshButtonsActiveness()
{//Before I do this, I first need to think what buttons I even have. 
    return ErrorCode::OK;
}

ErrorCode PlayerStateIndicatingElement::m_createButtons()
{
    std::vector<ButtonType> buttonTypes = m_buttonTypes();
    int i = 0;
    for(const auto& buttonType: buttonTypes){
         int maxYSteps = 8; //This should be an enum value or something, heavily related to some standard value of the game logic. For now, it is just a number.
        SDL_Rect rect {m_x + (i/maxYSteps)*BUTTON_WIDTH, m_y + (i % maxYSteps) * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
        m_buttons.emplace_back(std::make_unique<HoverableButtonElement>(m_playerID, rect, m_stringForPlayerID(m_playerID, false), m_colors.basicColors, m_colors.furtherColors, GameObject::DEFAULT_LAYER, Activeness::PASSIVE));
        i++;
        m_buttonMap[buttonType] = m_buttons.back().get();
    }
    return ErrorCode::OK;
}

bool PlayerStateIndicatingElement::m_showHoveredState()
{
    return false;
}

std::map<PlayerStateIndicatingElement::ButtonType, std::string> PlayerStateIndicatingElement::m_generateButtonTexts() const{
    try{
    std::map<PlayerStateIndicatingElement::ButtonType, std::string> texts; 
    const GameData& gameData = m_gameDataGiver.m_GameData();
    const Round::RoundData* roundData = m_gameDataGiver.m_retrieveCurrentRoundData();
    if(!roundData)
        throw std::runtime_error("No current round data available!");
    PlayerID explainingPlayerID = roundData->explainingPlayer;
    PlayerID guessingPlayerID = roundData->guessingPlayer;
    if(explainingPlayerID == PlayerID::DUMMY_PLAYER || guessingPlayerID == PlayerID::DUMMY_PLAYER)
        throw std::runtime_error("Invalid player IDs!");
    texts[PlayerStateIndicatingElement::ButtonType::WHICH_ROUND] = "Round " + std::to_string(gameData.roundHistory.size() + 1);
    texts[PlayerStateIndicatingElement::ButtonType::HOW_MANY_POINTS] = "Points: " + std::to_string(m_player->m_Data().points);
    texts[PlayerStateIndicatingElement::ButtonType::WHOS_EXPLAINING] = "Explaining: " + m_stringForPlayerID(explainingPlayerID);
    texts[PlayerStateIndicatingElement::ButtonType::WHOS_GUESSING] = "Guessing: " + m_stringForPlayerID(guessingPlayerID);
    texts[PlayerStateIndicatingElement::ButtonType::SCALES_COMMITTED] = roundData->m_areAllScalesCommitted() ? "(Committed)" : "(Not committed)";
    auto pointsTable = roundData->m_determinePoints(false); // Determine the expected points and look at the player's entry. Hopefully, map::at() will not throw, but just in case we'll catch any exceptions.
    texts[PlayerStateIndicatingElement::ButtonType::EXPECTED_POINTS] = "Expected Points: " + std::to_string(pointsTable.at(m_player->m_Data().playerID));
        return texts;
    }
    catch (...) {
        throw;
    }
}

HoverableCompositeButtonElement::ColorSet PlayerStateIndicatingElement::s_standardColors(){
    return Comp::ColorSet{
            ClassicUnhoverableButtonElement::ColorSet{
                Graphics::turquoiseColor(),
                Graphics::shieldGreyColor(),
                Graphics::brownColor(),
        },
        HoverableButtonElement::ColorSetExtension{
            SDL_Color{170, 255, 100},
            SDL_Color{210, 210, 150},
            SDL_Color{235,235,190},
            SDL_Color{255, 150, 225},
            SDL_Color{150, 150, 255},
            SDL_Color{255,150, 0}
        }
    };
}

std::string PlayerStateIndicatingElement::m_stringForPlayerID(PlayerID playerID, bool considerSelf) const{
    if(considerSelf && playerID == m_player->m_Data().playerID)
        return "You";
    return "Player no " + std::to_string((int) playerID) + ": " + m_gameDataGiver.m_PlayerNameFor(playerID);
}

ErrorCode PlayerStateIndicatingElement::m_putIntoEffect(const Suggestor::Suggestion& suggestion){
    if(suggestion.pointsIncrease != 0){
        m_player->m_data.points += suggestion.pointsIncrease;
        return ErrorCode::OK;
    }
    return ErrorCode::IRRELEVANT_SUGGESTION;
}

bool PlayerStateIndicatingElement::m_isRelevant(const Suggestor::Suggestion& suggestion){
    return suggestion.pointsIncrease != 0 && suggestion.playerID == m_player->m_Data().playerID;
}

std::vector<PlayerStateIndicatingElement::ButtonType> PlayerStateIndicatingElement::m_buttonTypes(){
    return {
        ButtonType::PLAYER_NAME_AND_ID,
        ButtonType::WHICH_ROUND,
        ButtonType::HOW_MANY_POINTS,
        ButtonType::WHOS_EXPLAINING,
        ButtonType::WHOS_GUESSING,
        ButtonType::SCALES_COMMITTED,
        ButtonType::EXPECTED_POINTS
    };
}
