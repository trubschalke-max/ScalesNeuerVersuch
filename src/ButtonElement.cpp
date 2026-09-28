#include "ButtonElement.hpp"
#include "Graphics.hpp"
#include <iostream>

ButtonElement::ButtonElement(int x, int y, int width, int height, std::string text, GameObject::Layer layer, Activeness activeness): GameObject(x, y, layer), m_width(width), m_height(height), m_textDisplayed(text), m_activeness(activeness)
{

}

ButtonElement::ButtonElement(SDL_Rect shapeAndPlace, std::string text,GameObject::Layer layer, ButtonElement::Activeness activeness):ButtonElement(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, layer, activeness){}

ButtonElement::~ButtonElement()
{

}

ErrorCode ButtonElement::m_draw(SDL_Renderer* renderer){//Better redo this. In particular, make sure the sdl rect has the right size.
    ErrorCode whatToReturn = ErrorCode::OK;
    SDL_Rect rect = Graphics::printMultipleLines(m_textDisplayed, m_x, m_y, m_currentTextColor(), renderer, m_currentBackgroundColor());
    if(rect.w == 0 || rect.h == 0)
        whatToReturn = ErrorCode::INVALID_RECT;
        
    if(whatToReturn != ErrorCode::OK)
        std::cerr<<whatToReturn<<", SDL-Error: "<<SDL_GetError()<< ", buttonText = "<<m_buttonData().text<<", rect = "<<m_buttonData().shapeAndPlace<<std::endl;
    return whatToReturn;
}

SDL_Rect ButtonElement::m_areaRect(){
    return SDL_Rect{m_x, m_y, m_width, m_height};
}

bool ButtonElement::m_isHitBy(int x, int y){
    SDL_Rect area = m_areaRect();
    return Graphics::isWithinRect(x, y, area);
}