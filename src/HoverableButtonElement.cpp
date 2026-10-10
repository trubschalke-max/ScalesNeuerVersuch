#include "HoverableButtonElement.hpp"
#include "SuggestSupport/HoverableButtonElementDisplayingSupporter.hpp"

#define ScaleLike HoverableButtonElement
#define BaseButton ClassicUnhoverableButtonElement
using ObjectData::Activeness;

ScaleLike::ScaleLike(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, ColorSetExtension furtherColors, GameObject::Layer layer,ObjectData::Activeness activeness): BaseButton(x, y, width, height, text, BaseButton::ColorSet{backgroundColorWhenActive, backgroundColorWhenPassive, textColor}, layer, activeness), m_furtherColors(furtherColors) 
{
    m_displayingSupporter = std::make_unique<HoverableButtonElementDisplayingSupporter>(this);
}
ScaleLike::ScaleLike(int x, int y, int width, int height, std::string text,  ColorSet colors, ColorSetExtension furtherColors, GameObject::Layer layer,ObjectData::Activeness activeness): ScaleLike(x, y, width, height, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, furtherColors, layer, activeness){}

ScaleLike::ScaleLike(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, ColorSetExtension furtherColors, GameObject::Layer layer,ObjectData::Activeness activeness): ScaleLike(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, colors.backgroundColorWhenActive, colors.backgroundColorWhenPassive, colors.textColor, furtherColors, layer, activeness){}

ScaleLike::~ScaleLike()
{

}

SDL_Color ScaleLike::m_currentBackgroundColor(){
    if(m_activeness == Activeness::HOVERED_OVER_AND_ACTIVE || m_activeness == Activeness::ACTIVE)
        return m_colors.backgroundColorWhenActive;
    if(m_activeness == Activeness::HOVERED_OVER_AND_PASSIVE)
        return m_furtherColors.m_backgroundColorWhenHovered;
    if(m_activeness == Activeness::FORBIDDEN)
        return m_furtherColors.m_backgroundColorWhenForbidden;
    if(m_activeness == Activeness::PERMANENTLY_SELECTED)
        return m_furtherColors.m_colorWhenPermanentlyActive;
    if(m_activeness == Activeness::ACTIVENESS_ERROR)
        return s_errorColor();
    if(m_activeness == Activeness::SECRETLY_ACTIVE)
        return m_furtherColors.m_colorWhenSecretlyActive;
    if(m_activeness == Activeness::SECRETLY_PASSIVE)
        return m_furtherColors.m_colorWhenSecretlyPassive;
    if(m_activeness == Activeness::PASSIVE)
        return m_colors.backgroundColorWhenPassive;
    else return BaseButton::m_currentBackgroundColor();
}