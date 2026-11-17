#include "Player.hpp"
#include <algorithm>

Player::Player(ObjectData::PlayerID playerID, const std::string playerName):m_data{playerID, playerName}
{
    m_playerUI = std::make_unique<PlayerUI>(playerName, this);
}

Player::~Player()
{

}

ErrorCode Player::m_addObject(GameObject* obj){
    if(!obj)
        return ErrorCode::NULLPOINTER_EXCEPTION;
    m_associatedObjects.push_back(std::unique_ptr<GameObject>(obj));
    return m_Drawing()->m_add(obj); // Add the object to the drawing associated with the player UI.
}

ErrorCode Player::m_removeObject(GameObject* obj){
    if(!obj)
        return ErrorCode::NULLPOINTER_EXCEPTION;
    auto it = std::remove_if(m_associatedObjects.begin(), m_associatedObjects.end(),
                             [obj](const std::unique_ptr<GameObject>& o){ return o.get() == obj; });
    if(it != m_associatedObjects.end()){
        m_associatedObjects.erase(it, m_associatedObjects.end());
        return m_Drawing()->m_remove(obj); // Remove the object from the drawing associated with the player UI.
    }
    return ErrorCode::REMOVAL_OF_NONEXISTENT_VALUE;
}

void Player::m_clearObjects(){
        m_associatedObjects.clear();
        return this->m_playerUI->m_clear(); // Clear the objects from the drawing associated with the player UI.
}

ErrorCode Player::m_addRecipe(Recipe* recipe){
    ErrorCode error = ErrorCode::OK;
    if(!recipe)
        return ErrorCode::NULLPOINTER_EXCEPTION;
        auto factory = GameObjectFactory::getInstance();
    GameObject* obj = factory->m_createGameObject(*recipe);
    if(!obj)
        error = ErrorCode::GAME_OBJECT_CREATION_FAILED;
    SDL_Point position = m_playerUI->m_figureOutPosition(obj, recipe->type, true);
    obj->m_setPosition(position.x, position.y); // Set the position of the created object based on the calculated position.
    checkForError(m_addObject(obj), error); // Add the created object to the player UI and associated drawing.
    return error;
}

ErrorCode Player::m_removeGameObjects(GameObject::Longevity longevity){
    for(auto it = m_associatedObjects.begin(); it != m_associatedObjects.end(); ){
        if((*it)->m_Longevity() == longevity){
            m_Drawing()->m_remove(it->get());
            it = m_associatedObjects.erase(it);
        }
        else{
            ++it;
        }
    }
    return ErrorCode::OK;
}

void Player::m_resetRoundBasedShiftsInUI(){
    if(m_playerUI)
        m_playerUI->m_resetRoundBasedShifts();
}

ErrorCode Player::m_prepareForNewRound(){
    m_resetRoundBasedShiftsInUI();
    return ErrorCode::OK;
}