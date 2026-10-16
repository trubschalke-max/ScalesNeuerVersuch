#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H
#pragma once

#include "ObjectData.hpp"
#include "GameObject.hpp"
#include "HoverableButtonElement.hpp"

class Scale; class ExplainableSet;

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
        SDL_Rect shapeAndPlace;
        ObjectData::PlayerID playerID = ObjectData::PlayerID::PLAYER_NONE;
        std::string text = "";
        GameObject::Layer layer = GameObject::DEFAULT_LAYER;
        ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE;
    };
    class InvalidRecipeException: public std::logic_error {
    public:
        InvalidRecipeException(const std::string& message) : std::logic_error(message) {}
    };
public:
static GameObjectFactory* getInstance(); //Returns the singleton instance of the factory.
GameObject* m_createGameObject(const Recipe& recipe); //Creates a game object based on the provided recipe. 
private:
    GameObjectFactory();
    ~GameObjectFactory();
};


#endif