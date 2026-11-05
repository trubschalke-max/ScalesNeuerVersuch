#ifndef PLAYERSTATEINDICATINGELEMENT_H
#define PLAYERSTATEINDICATINGELEMENT_H
#pragma once

#include "HoverableCompositeButtonElement.hpp"
#include "SuggestSupport/PlayerStateIndicatingElementDisplayingSupporter.hpp"
#include "ObjectData.hpp"
#include "GameDataGiver.hpp"
#include <map>

class Player;


class PlayerStateIndicatingElement: public HoverableCompositeButtonElement
//This is intended to visually show the player's state. I want to implement a system of points the player can earn by guessing and explaining well. I need buttons for:
//Which round it is
//Points the player has earned (in the far future, a right click might open an explanation for the point system)
//Who's explaining
//Who's guessing
{
    enum{BUTTON_WIDTH = 300, BUTTON_HEIGHT = 15};
public:
using PlayerID = ObjectData::PlayerID;
enum class ButtonType
{
    PLAYER_NAME_AND_ID = 0,
    WHICH_ROUND = 1,
    HOW_MANY_POINTS = 2,
    WHOS_EXPLAINING = 3,
    WHOS_GUESSING = 4, 
    SCALES_COMMITTED = 5, 
    EXPECTED_POINTS = 6
};

private:
    std::map<PlayerStateIndicatingElement::ButtonType, std::string> m_generateButtonTexts() const;
    Player* m_player; //I do not make the player const, because the element is intended to apply a suggestion for increase of points.
    GameDataGiver& m_gameDataGiver;
    std::string m_stringForPlayerID(PlayerID playerID, bool considerSelf = true) const;
    ErrorCode m_putIntoEffect(const Suggestor::Suggestion& suggestion) override; //This can be pretty dangerous, as it directly modifies the player's points. Use with caution.
    std::map<PlayerStateIndicatingElement::ButtonType, HoverableButtonElement*> m_buttonMap; //In theory, this is redundant.
    std::vector<ButtonType> m_buttonTypes();

public:
    PlayerStateIndicatingElement(GameDataGiver& game, Player* player, int x, int y);
    virtual ~PlayerStateIndicatingElement();
    ErrorCode m_refreshButtonsActiveness() override;
    ErrorCode m_createButtons() override;
    bool m_showHoveredState() override;
    std::map<PlayerStateIndicatingElement::ButtonType, std::string> m_ButtonTexts() const; 
    static HoverableCompositeButtonElement::ColorSet s_standardColors();
    bool m_isRelevant(const Suggestor::Suggestion& suggestion) override;
    friend class PlayerStateIndicatingElementDisplayingSupporter;
};

#endif