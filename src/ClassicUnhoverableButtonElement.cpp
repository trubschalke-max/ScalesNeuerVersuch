#include "ClassicUnhoverableButtonElement.hpp"
#include "ButtonSuggestor.hpp"
using ObjectData::Activeness;

ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, GameObject::Layer layer,ObjectData::Activeness activeness): ButtonElement(x, y, width, height, text, layer, activeness), m_colors{backgroundColorWhenActive, backgroundColorWhenPassive, textColor}
{m_suggestor = std::make_unique<ButtonSuggestor>(this);}
ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text,  ColorSet colors, GameObject::Layer layer,ObjectData::Activeness activeness): ClassicUnhoverableButtonElement(x, y, width, height, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, layer, activeness){}

ClassicUnhoverableButtonElement::ClassicUnhoverableButtonElement(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, GameObject::Layer layer,ObjectData::Activeness activeness): ClassicUnhoverableButtonElement(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, layer, activeness){}

ClassicUnhoverableButtonElement::~ClassicUnhoverableButtonElement()
{

}

SDL_Color ClassicUnhoverableButtonElement::m_currentBackgroundColor(){
    switch(m_activeness){
        case Activeness::PASSIVE: return m_colors.backgroundColorWhenPassive;
        case Activeness::ACTIVE: return m_colors.backgroundColorWhenActive;
        default: return s_errorColor();
    }
}

SDL_Color ClassicUnhoverableButtonElement::m_currentTextColor(){
    return m_colors.textColor;
}

