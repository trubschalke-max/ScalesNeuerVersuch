#include "HoverableButtonElement.hpp"

#define ScaleLike HoverableButtonElement
#define BaseButton ClassicUnhoverableButtonElement

ScaleLike::ScaleLike(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, SDL_Color backgroundColorWhenHovered, GameObject::Layer layer, ButtonElement::Activeness activeness): BaseButton(x, y, width, height, text, BaseButton::ColorSet{backgroundColorWhenActive, backgroundColorWhenPassive, textColor}, layer, activeness), m_backgroundColorWhenHovered(backgroundColorWhenHovered) 
{}
ScaleLike::ScaleLike(int x, int y, int width, int height, std::string text,  ColorSet colors, SDL_Color backgroundColorWhenHovered, GameObject::Layer layer, ButtonElement::Activeness activeness): ScaleLike(x, y, width, height, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, backgroundColorWhenHovered, layer, activeness){}

ScaleLike::ScaleLike(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, SDL_Color backgroundColorWhenHovered, GameObject::Layer layer, ButtonElement::Activeness activeness): ScaleLike(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, backgroundColorWhenHovered, layer, activeness){}

ScaleLike::~ScaleLike()
{

}

SDL_Color ScaleLike::m_currentBackgroundColor(){
    if(m_activeness == Activeness::HOVERED_OVER_AND_ACTIVE)
        return m_colors.backgroundColorWhenActive;
    if(m_activeness == Activeness::HOVERED_OVER_AND_PASSIVE)
        return m_backgroundColorWhenHovered;
    else return BaseButton::m_currentBackgroundColor();
}