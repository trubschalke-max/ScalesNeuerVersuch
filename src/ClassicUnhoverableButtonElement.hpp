#ifndef CLASSICUNHOVERABLEBUTTONELEMENT_H
#define CLASSICUNHOVERABLEBUTTONELEMENT_H

#pragma once

#include "ButtonElement.hpp"

class ClassicUnhoverableButtonElement: public ButtonElement{
public: 
    struct ColorSet{
    SDL_Color backgroundColorWhenActive;
    SDL_Color backgroundColorWhenPassive;
    SDL_Color textColor;
    };
protected:
    ColorSet m_colors;
public:
    ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text, SDL_Color backgroundColorWhenActive, SDL_Color backgroundColorWhenPassive, SDL_Color textColor, GameObject::Layer layer = DEFAULT_LAYER, ButtonElement::Activeness activeness = PASSIVE);
    ClassicUnhoverableButtonElement(int x, int y, int width, int height, std::string text, ColorSet colors, GameObject::Layer layer = DEFAULT_LAYER, ButtonElement::Activeness activeness = PASSIVE);
    ClassicUnhoverableButtonElement(SDL_Rect shapeAndPlace, std::string text, ColorSet colors, GameObject::Layer layer = DEFAULT_LAYER, ButtonElement::Activeness activeness = PASSIVE);
    
    virtual SDL_Color m_currentBackgroundColor();
    virtual SDL_Color m_currentTextColor();
    virtual ~ClassicUnhoverableButtonElement();

};

typedef ClassicUnhoverableButtonElement Button;

#endif