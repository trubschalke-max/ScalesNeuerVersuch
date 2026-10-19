#include "HoverableCompositeButtonElement.hpp"

HoverableCompositeButtonElement::HoverableCompositeButtonElement(ObjectData::PlayerID playerID, int x, int y, Layer layer): GameObject(playerID, x, y, layer)
{
    m_displayingSupporter = std::make_unique<HoverableCompositeButtonElementDisplayingSupporter>(this);
}

HoverableCompositeButtonElement::~HoverableCompositeButtonElement()
{

}

bool Comp::m_isHitBy(int x, int y){
    for(auto& button: m_buttons){
        if(button->m_isHitBy(x,y)){
            return true;
        }
    }
    return false;
}

ErrorCode Comp::m_draw(SDL_Renderer* renderer){
    ErrorCode error = ErrorCode::OK;
    for(auto& button: m_buttons){
        error = button->m_draw(renderer);
        if(error != ErrorCode::OK){
        throw std::runtime_error("Drawing a scale has failed!");
        }
    }
    return error;
}

bool Comp::m_showHoveredState(){
    return true;
}

void Comp::m_setPosition(int x, int y){
    for(auto& button: m_buttons){
        int buttonDeltaX = button->m_X() - m_x;//Compute the relative position of the button to the composite element.
        int buttonDeltaY = button->m_Y() - m_y;
        button->m_setPosition(x + buttonDeltaX, y + buttonDeltaY); 
    }
    m_x = x;
    m_y = y;
}