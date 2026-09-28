#ifndef SCALEELEMENT_H
#define SCALEELEMENT_H

#pragma once

#include "GameObject.hpp"
#include "Scale.hpp"
#include "HoverableButtonElement.hpp"
#include <vector>

class ScaleElement: public GameObject
{
    public: struct ColorSet{
        ClassicUnhoverableButtonElement::ColorSet basicColors;
        SDL_Color colorWhenHoveredOver;
    };
    private:
    Scale* m_associatedScale;
    ErrorCode m_createButtons();
    std::vector<HoverableButtonElement> m_buttons;
    ErrorCode m_associatedError = ErrorCode::OK;
    ColorSet m_colors;


    public:
    ScaleElement(int x, int y, Scale* scale);
    virtual ~ScaleElement();
    
    virtual bool m_isHitBy(int x, int y) override;
    virtual ErrorCode m_draw(SDL_Renderer* renderer) override;
    int m_Width(){return 120;} //Some value for later revisiting
    int m_HeightPerElement(){return 30;} //Some value for later revisiting
    
    static ColorSet s_standardColors();
};

#endif