#include "ButtonElement.hpp"
#include "Graphics.hpp"
#include <iostream>

ButtonElement::ButtonElement(int x, int y, int width, int height, std::string text, GameObject::Layer layer, bool isActive): GameObject(x, y, layer), m_width(width), m_height(height), m_textDisplayed(text), m_isActive(isActive)
{

}

ButtonElement::ButtonElement(SDL_Rect shapeAndPlace, std::string text,GameObject::Layer layer, bool isActive):ButtonElement(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, layer, isActive){}

ButtonElement::~ButtonElement()
{

}

ErrorCode ButtonElement::m_draw(SDL_Renderer* renderer){
    ErrorCode whatToReturn = ErrorCode::OK;
    std::cout<<Graphics::printMultipleLines(m_textDisplayed, m_x, m_y, m_currentTextColor(), renderer, m_currentBackgroundColor())<<std::endl;

    return whatToReturn;
}