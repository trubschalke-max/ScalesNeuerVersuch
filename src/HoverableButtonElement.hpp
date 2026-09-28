#ifndef HOVERABLEBUTTONELEMENT_H
#define HOVERABLEBUTTONELEMENT_H

#pragma once

#include "ClassicUnhoverableButtonElement.hpp"

class HoverableButtonElement: public ClassicUnhoverableButtonElement
{
protected: 
    SDL_Color m_backgroundColorWhenHovered;
    bool m_drawAsHoveredOver = false; 
private:

public:
    HoverableButtonElement(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, SDL_Color backgroundColorWhenHovered, GameObject::Layer layer = DEFAULT_LAYER, ButtonElement::Activeness activeness = PASSIVE);
    HoverableButtonElement(int x, int y, int width, int height, std::string text, ColorSet colors, SDL_Color backgroundColorHovered, GameObject::Layer layer = DEFAULT_LAYER, ButtonElement::Activeness activeness = PASSIVE);
    HoverableButtonElement(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, SDL_Color backgroundColorHovered, GameObject::Layer layer = DEFAULT_LAYER, ButtonElement::Activeness activeness = PASSIVE);
    virtual ~HoverableButtonElement();
    
    
    virtual SDL_Color m_currentBackgroundColor() override;
};

#endif