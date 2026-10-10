#ifndef HOVERABLEBUTTONELEMENTDISPLAYINGSUPPORTER_H
#define HOVERABLEBUTTONELEMENTDISPLAYINGSUPPORTER_H

#pragma once
#include "HoverableButtonElement.hpp"

class HoverableButtonElementDisplayingSupporter: public DisplayingSupporter
{
private:
    HoverableButtonElement* m_button;
public:
    HoverableButtonElementDisplayingSupporter(HoverableButtonElement* button);
    virtual ~HoverableButtonElementDisplayingSupporter();
    virtual ErrorCode m_changeDisplayingDueTo(const SDL_Event& event) override; 
    virtual bool m_isRelevantForDisplaying(const SDL_Event& event) override; 
};

#endif