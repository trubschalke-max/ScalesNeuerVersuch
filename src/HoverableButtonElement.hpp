#ifndef HOVERABLEBUTTONELEMENT_H
#define HOVERABLEBUTTONELEMENT_H

#pragma once

#include "ClassicUnhoverableButtonElement.hpp"

class HoverableButtonElement: public ClassicUnhoverableButtonElement
{
public:
    struct ColorSetExtension{
        SDL_Color m_backgroundColorWhenHovered;
        SDL_Color m_backgroundColorWhenForbidden;
        SDL_Color m_colorWhenPermanentlyActive;
        SDL_Color m_colorWhenSecretlyActive;
        SDL_Color m_colorWhenSecretlyPassive;
    };
protected: 
    ColorSetExtension m_furtherColors;
private:

public:
    HoverableButtonElement(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, ColorSetExtension furtherColors, GameObject::Layer layer = DEFAULT_LAYER, ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE);
    HoverableButtonElement(int x, int y, int width, int height, std::string text, ColorSet colors, ColorSetExtension furtherColors, GameObject::Layer layer = DEFAULT_LAYER,ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE);
    HoverableButtonElement(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, ColorSetExtension furtherColors, GameObject::Layer layer = DEFAULT_LAYER,ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE);
    virtual ~HoverableButtonElement();
    HoverableButtonElement(HoverableButtonElement&&) = default;
    
    virtual SDL_Color m_currentBackgroundColor() override;

};

#endif