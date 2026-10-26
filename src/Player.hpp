#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "ObjectData.hpp"
#include "PlayerEventHandler.hpp"
#include <memory>
#include "Drawing.hpp"
#include "SuggestSupport/Suggestor.hpp"
#include "GameObject.hpp"
#include "Window.hpp"
#include "PlayerUI.hpp"
#include "myExceptions.hpp"
#include "GameObjectFactory.hpp"


class Player //This class manages its own window and event handling, as well as it contains some playerData info. The player is not supposed to implement much game logic. Picture it as both a player's board and their hand.
{
    using GameObjectType = ObjectData::GameObjectType;
    using Recipe = GameObjectFactory::Recipe;
public: struct PlayerData{
    ObjectData::PlayerID playerID;
    std::string playerName;
};
private: 
    std::unique_ptr<Suggestor::Suggestion> m_currentSuggestion;
    std::vector<std::unique_ptr<GameObject>> m_associatedObjects; 
    PlayerData m_data;
    std::unique_ptr<PlayerUI> m_playerUI;
public:
    Player(ObjectData::PlayerID playerID, const std::string playerName);
    ~Player();
    const PlayerData& m_Data(){return m_data;}
    Drawing* m_Drawing(){if(m_playerUI) return &m_playerUI->m_associatedDrawing; throw NullPointerException("Player", "m_Drawing()");}
    ErrorCode m_addObject(GameObject* obj); //This will also add the object to the drawing associated with the player UI.
    ErrorCode m_removeObject(GameObject* obj); //This will also remove the object from the drawing associated with the player UI.
    void m_clearObjects(); //This will also clear the objects from the drawing associated with the player UI.
    ErrorCode m_removeGameObjects(GameObject::Longevity longevity); //Removes all game objects associated with the player that have the specified longevity and clears them from the drawing. Note that for "smaller" longevities, those still need to be removed separately.
    ErrorCode m_addRecipe(Recipe* recipe); //Creates an object based on a recipe, and adds it to the UI.
    void m_resetRoundBasedShiftsInUI(); //This is not clean code, I know. But I don't know how else to do that.
    ErrorCode m_prepareForNewRound(); //In the future, this could potentially have some graphic implications or whatever.
};

#endif