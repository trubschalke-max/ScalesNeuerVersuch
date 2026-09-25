#include "ClassicUnhoverableButtonElement.hpp"


ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenNotActive, SDL_Color textColor, GameObject::Layer layer, bool isActive): ButtonElement(x, y, width, height, text, layer, isActive), m_colors{backgroundColorWhenActive, backgroundColorWhenNotActive, textColor}
{}
ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text,  ColorSet colors, GameObject::Layer layer, bool isActive): ClassicUnhoverableButtonElement(x, y, width, height, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenNotActive, colors.textColor, layer, isActive){}

ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, GameObject::Layer layer, bool isActive): ClassicUnhoverableButtonElement(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenNotActive, colors.textColor, layer, isActive){}

ClassicUnhoverableButtonElement::~ClassicUnhoverableButtonElement()
{

}

SDL_Color ClassicUnhoverableButtonElement::m_currentBackgroundColor(){
    if(m_isActive)
        return m_colors.backgroundColorWhenActive;
    else
        return m_colors.backgroundColorWhenNotActive;
}

SDL_Color ClassicUnhoverableButtonElement::m_currentTextColor(){
    return m_colors.textColor;
}

