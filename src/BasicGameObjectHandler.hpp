#ifndef BASICGAMEOBJECTHANDLER_H
#define BASICGAMEOBJECTHANDLER_H

#pragma once

#include "EventHandler.hpp"

class BasicGameObjectHandler: public EventHandler
{
protected:
    HandlingResult m_handleRelevantEvent(const SDL_Event& event, HandlingClass diagnosis) override;
    //virtual HandlingClass m_classify(const SDL_Event& event) override;
    virtual ErrorCode m_enter() override;

public:
    BasicGameObjectHandler(Drawing* whatToDraw);
    virtual ~BasicGameObjectHandler(); 
};

#endif