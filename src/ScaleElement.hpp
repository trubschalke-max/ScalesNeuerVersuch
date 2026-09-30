#ifndef SCALEELEMENT_H
#define SCALEELEMENT_H

#pragma once

#include "Scale.hpp"
#include "HoverableButtonElement.hpp"
#include <vector>

#define FURTHER_SCALE_BUTTONS_COUNT 2

class ScaleElement: public GameObject
{
    public: struct ColorSet{
        ClassicUnhoverableButtonElement::ColorSet basicColors;
        HoverableButtonElement::ColorSetExtension furtherColors;
    };
    private:
    Scale* m_associatedScale;
    ErrorCode m_createButtons();
    std::vector<HoverableButtonElement> m_buttons;
    ColorSet m_colors;
    int m_buttonIndexFor(Scale::ScaleValue val);
    ButtonElement::Activeness m_figureOutActiveness(Scale::ScaleValue whatFor);

    public:
    ScaleElement(int x, int y, Scale* scale);
    virtual ~ScaleElement();

    ErrorCode m_leftClick(int x, int y) override;
    
    Scale::ScaleValue m_valueAt(int x, int y); //This function recognizes the commit button and the undo button. If the value does not belong to a button, DUMMY_VALUE is being returned.
    virtual bool m_isHitBy(int x, int y) override;
    virtual ErrorCode m_draw(SDL_Renderer* renderer) override;
    int m_Width(){return 200;} //Some value for later revisiting
    int m_HeightPerElement(){return 30;} //Some value for later revisiting
    ErrorCode m_refreshButtonsActiveness();


    static ColorSet s_standardColors();
};

std::string toString (Scale::ScaleValue s);

class InvalidSelection: public std::exception{
public:
    Scale::ScaleValue s;
    InvalidSelection(Scale::ScaleValue ps): s(ps){}
    const char* what(){std::string str = "Invalid selection: " + toString(s); return str.c_str();}
};

#endif