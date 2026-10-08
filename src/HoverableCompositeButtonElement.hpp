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
    virtual ErrorCode m_createButtons() =  0;
public:
    HoverableCompositeButtonElement(int x, int y, Layer layer = Layer::DEFAULT_LAYER);
    virtual ~HoverableCompositeButtonElement();
    ErrorCode m_draw();
    friend class HoverableCompositeButtonElementDisplayingSupporter;
    virtual bool m_isHitBy(int x, int y) override;

};

typedef HoverableCompositeButtonElement Comp;

#endif