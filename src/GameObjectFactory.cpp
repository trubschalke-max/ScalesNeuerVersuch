#include "GameObjectFactory.hpp"
#include "ScaleElement.hpp"
#include "ExplainableSetElement.hpp"
#include "ClassicUnhoverableButtonElement.hpp"
#include "HoverableButtonElement.hpp"
#include "PlayerStateIndicatingElement.hpp"

GameObjectFactory::GameObjectFactory()
{

}

GameObjectFactory::~GameObjectFactory()
{

}

GameObjectFactory* GameObjectFactory::getInstance()
{
    static GameObjectFactory instance;
    return &instance;
}

GameObject* GameObjectFactory::m_createGameObjectRaw(const Recipe& recipe){
    switch(recipe.type){
        case GameObjectType::SCALE_ELEMENT:
            if(!recipe.scale)
                throw InvalidRecipeException("No scale");
            return new ScaleElement(recipe.playerID, recipe.shapeAndPlace.x, recipe.shapeAndPlace.y, recipe.scale, recipe.isForExplainingPlayer, recipe.layer);
        case GameObjectType::EXPLAINABLE_SET_ELEMENT:
            if(!recipe.exSet)
                throw InvalidRecipeException("No explainable set");
            return new ExplainableSetElement(recipe.playerID, recipe.shapeAndPlace.x, recipe.shapeAndPlace.y, recipe.exSet, recipe.isForExplainingPlayer, recipe.layer);
        case GameObjectType::CLASSIC_UNHOVERABLE_BUTTON:
            return new ClassicUnhoverableButtonElement(recipe.playerID, recipe.shapeAndPlace, recipe.text, recipe.colors, recipe.layer, recipe.activeness);
        case GameObjectType::HOVERABLE_BUTTON:
            return new HoverableButtonElement(recipe.playerID, recipe.shapeAndPlace, recipe.text, recipe.colors, recipe.furtherColors, recipe.layer, recipe.activeness);
        case GameObjectType::PLAYER_STATE_INDICATING_ELEMENT:
            return new PlayerStateIndicatingElement(*recipe.gameDataGiver, recipe.player, recipe.shapeAndPlace.x, recipe.shapeAndPlace.y);
        default:
            throw InvalidRecipeException("Invalid recipe type");
    }
}

GameObject* GameObjectFactory::m_createGameObject(const Recipe& recipe){
 try{
        GameObject* obj = m_createGameObjectRaw(recipe);
        if(!obj)
            throw InvalidRecipeException("Failed to create game object");
        obj->m_setLongevity(recipe.longevity);
        obj->m_setHandlingMode(recipe.handlingMode);
        return obj;
    } catch(const InvalidRecipeException& e){
        throw;
    }
    return nullptr;
}