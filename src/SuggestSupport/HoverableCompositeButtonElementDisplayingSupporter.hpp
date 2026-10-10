#ifndef HOVERABLECOMPOSITEBUTTONELEMENTDISPLAYINGSUPPORTER_H
#define HOVERABLECOMPOSITEBUTTONELEMENTDISPLAYINGSUPPORTER_H
#pragma once

#include "SuggestSupport/DisplayingSupporter.hpp"

class HoverableCompositeButtonElement;

class HoverableCompositeButtonElementDisplayingSupporter: public DisplayingSupporter
{

private:
    HoverableCompositeButtonElement* m_element;
public:
    HoverableCompositeButtonElementDisplayingSupporter(HoverableCompositeButtonElement* element);
    virtual ~HoverableCompositeButtonElementDisplayingSupporter();
    virtual ErrorCode m_changeDisplayingDueTo(const SDL_Event& event) override; //The expected results are either NOTHING_HAPPENED or MERELY_VISUAL_IMPLICATIONS.
    virtual bool m_isRelevantForDisplaying(const SDL_Event& event) override; 
};

typedef HoverableCompositeButtonElementDisplayingSupporter CompSupp;

#endif