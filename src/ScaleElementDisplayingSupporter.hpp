#ifndef SCALEELEMENTDISPLAYINGSUPPORTER_H
#define SCALEELEMENTDISPLAYINGSUPPORTER_H

#pragma once
#include "ScaleElement.hpp"

class ScaleElementDisplayingSupporter: public DisplayingSupporter
{

private:
    ScaleElement* m_element;
public:
    ScaleElementDisplayingSupporter(ScaleElement* element);
    virtual ~ScaleElementDisplayingSupporter();
    virtual ErrorCode m_changeDisplayingDueTo(const SDL_Event& event) override; //The expected results are either NOTHING_HAPPENED or MERELY_VISUAL_IMPLICATIONS.
    virtual bool m_isRelevantForDisplaying(const SDL_Event& event) override; 
};

#endif