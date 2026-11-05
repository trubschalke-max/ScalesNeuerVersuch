#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H
#pragma once

#include "ObjectData.hpp"
#include "GameObject.hpp"
#include "HoverableButtonElement.hpp"

class Scale; class ExplainableSet; class GameDataGiver; class Player;

class GameObjectFactory{ //This shall be a singleton class.
public: 
    using GameObjectType = ObjectData::GameObjectType;
    struct Recipe{
        GameObjectType type;
        Scale* scale;
        ExplainableSet* exSet;
        bool isForExplainingPlayer;
        ClassicUnhoverableButtonElement::ColorSet colors;
        HoverableButtonElement::ColorSetExtension furtherColors;
        SDL_Rect shapeAndPlace{GameObject::POSITION_UNDEFINED, GameObject::POSITION_UNDEFINED, 0, 0};
        ObjectData::PlayerID playerID = ObjectData::PlayerID::PLAYER_NONE;
        std::string text = "";
        GameObject::Layer layer = GameObject::DEFAULT_LAYER;
        ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE;
        GameDataGiver* gameDataGiver;
        GameObject::Longevity longevity = GameObject::ONE_ROUND_LONG;
        GameObject::HandlingMode handlingMode = GameObject::HandlingMode::SUGGEST; 
        Player* player = nullptr;
    };
    class InvalidRecipeException: public std::logic_error {
    public:
        InvalidRecipeException(const std::string& message) : std::logic_error(message) {}
    };
private:
    GameObject* m_createGameObjectRaw(const Recipe& recipe); //Creates a game object based on the provided recipe. 
public:
    GameObject* m_createGameObject(const Recipe& recipe); //Creates a game object based on the provided recipe, with additional checks and handling. 
static GameObjectFactory* getInstance(); //Returns the singleton instance of the factory.
private:
    GameObjectFactory();
    ~GameObjectFactory();
};


#endif