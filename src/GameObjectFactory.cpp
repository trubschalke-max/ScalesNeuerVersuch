#include "GameObjectFactory.hpp"
#include "ScaleElement.hpp"
#include "ExplainableSetElement.hpp"
#include "ClassicUnhoverableButtonElement.hpp"
#include "HoverableButtonElement.hpp"

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

GameObject* GameObjectFactory::m_createGameObject(const Recipe& recipe){
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
        default:
            throw InvalidRecipeException("Invalid recipe type");
    }
}