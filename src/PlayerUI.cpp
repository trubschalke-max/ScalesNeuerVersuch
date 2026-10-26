#include "PlayerUI.hpp"
#include "Window.hpp"
#include <iostream>
#include "Player.hpp"
#include "ScaleElement.hpp"
#include "ExplainableSetElement.hpp"

PlayerUI::PlayerUI(std::string playerName, Player* player): m_playerName(playerName), m_associatedWindow(playerName), m_associatedDrawing(nullptr), m_associatedPlayer(player)
{
    if(!m_associatedPlayer)
        throw std::invalid_argument("Associated player cannot be null.");
    ErrorCode error = m_associatedWindow.m_init();
    if(error != ErrorCode::OK){
        std::cerr << "Error initializing associated window: " << toString(error) << std::endl;
        throw std::runtime_error("Failed to initialize associated window.");
    }
    m_associatedDrawing.m_setRenderer(m_associatedWindow.m_Renderer());
}

PlayerUI::~PlayerUI()
{

}

SDL_Point PlayerUI::m_figureOutPosition(GameObject* obj, ObjectData::GameObjectType type, bool remember){
    SDL_Point position = {0, 0};
    ScaleElement* scaleElement;
    switch(type){
        case ObjectData::GameObjectType::SCALE_ELEMENT:
         scaleElement = dynamic_cast<ScaleElement*>(obj);
            if(!scaleElement)
                throw std::invalid_argument("Failed to cast GameObject to ScaleElement.");
            position.x = m_nextScaleElementX; // Use the next available X position for scale elements.
            position.y = 0;
            if(remember)
                m_nextScaleElementX += scaleElement->m_Width() + 20; // Update the next X position for scale elements. The 20 is a magic number for some clearance.
            break;
        case ObjectData::GameObjectType::EXPLAINABLE_SET_ELEMENT: //I'm assuming there can only be one.
            // Example logic: position explainable set element below
            position.x = 0; 
            position.y = 450; 
            break;
        default:
            throw std::invalid_argument("Unknown or not supported GameObjectType");
    }
    return position;
}

void PlayerUI::m_clear(){
    m_associatedDrawing.m_clear();
    m_nextScaleElementX = 0;
}

void PlayerUI::m_resetRoundBasedShifts(){
    m_nextScaleElementX = 0;
}