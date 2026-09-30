#include "GameObject.hpp"

GameObject::GameObject(int x, int y, Layer layer): m_x(x), m_y(y), m_layer(layer){}

ErrorCode GameObject::m_onlyHandleLeftClicks(const SDL_Event& event){
    if(event.type != SDL_MOUSEBUTTONDOWN){
        return ErrorCode::NOTHING_HAPPENED;
    }
    if(!m_isHitBy(event.button.x, event.button.y))
        return ErrorCode::NOT_ACTUALLY_CLICKED;
    if(event.button.button == SDL_BUTTON_LEFT)
        return m_leftClick(event.button.x,event.button.y);
    else return ErrorCode::NOTHING_HAPPENED;
}

ErrorCode GameObject::m_handleEvent(const SDL_Event& event){
    return m_onlyHandleLeftClicks(event);
}

ErrorCode GameObject::m_leftClick(int x, int y){
    return ErrorCode::NOTHING_HAPPENED;
}