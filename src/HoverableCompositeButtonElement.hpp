#ifndef HOVERABLECOMPOSITEBUTTONELEMENT_H
#define HOVERABLECOMPOSITEBUTTONELEMENT_H
#pragma once

#include "GameObject.hpp"
#include "HoverableButtonElement.hpp"
#include "SuggestSupport/HoverableCompositeButtonElementDisplayingSupporter.hpp"

class HoverableCompositeButtonElement: public GameObject //Currently, this is merely a helper class for the displaying support. 
{
public:
    struct ColorSet{
        ClassicUnhoverableButtonElement::ColorSet basicColors;
        HoverableButtonElement::ColorSetExtension furtherColors;
    };

protected:
    std::vector<std::unique_ptr<HoverableButtonElement>> m_buttons;
    ColorSet m_colors;
    virtual ErrorCode m_createButtons() =  0; //This has to be called in the constructor of the derived (!) class.
    virtual bool m_showHoveredState();//This function is intended to be overridden in derived classes, and it will be used to determine whether the hovered state should be displayed or not. The default implementation returns true, indicating that the hovered state should be displayed.

public:
    HoverableCompositeButtonElement(ObjectData::PlayerID playerID, int x, int y, Layer layer = Layer::DEFAULT_LAYER);
    virtual ~HoverableCompositeButtonElement();
    ErrorCode m_draw(SDL_Renderer* renderer);
    friend class HoverableCompositeButtonElementDisplayingSupporter;
    virtual bool m_isHitBy(int x, int y) override;
    void m_setPosition(int x, int y) override; //Sets the position of the composite button element and updates the positions of its sub-buttons.

};

typedef HoverableCompositeButtonElement Comp;

#endif