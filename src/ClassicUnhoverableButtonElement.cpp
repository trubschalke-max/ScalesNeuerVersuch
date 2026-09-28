#include "ClassicUnhoverableButtonElement.hpp"


ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, GameObject::Layer layer, ButtonElement::Activeness activeness): ButtonElement(x, y, width, height, text, layer, activeness), m_colors{backgroundColorWhenActive, backgroundColorWhenPassive, textColor}
{}
ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text,  ColorSet colors, GameObject::Layer layer, ButtonElement::Activeness activeness): ClassicUnhoverableButtonElement(x, y, width, height, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, layer, activeness){}

ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, GameObject::Layer layer, ButtonElement::Activeness activeness): ClassicUnhoverableButtonElement(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, layer, activeness){}

ClassicUnhoverableButtonElement::~ClassicUnhoverableButtonElement()
{

}

SDL_Color ClassicUnhoverableButtonElement::m_currentBackgroundColor(){
    switch(m_activeness){
        case PASSIVE: return m_colors.backgroundColorWhenPassive;
        case ACTIVE: return m_colors.backgroundColorWhenActive;
        default: return s_errorColor();
    }
}

SDL_Color ClassicUnhoverableButtonElement::m_currentTextColor(){
    return m_colors.textColor;
}

