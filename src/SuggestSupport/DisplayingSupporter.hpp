#ifndef DISPLAYINGSUPPORTER_H
#define DISPLAYINGSUPPORTER_H

#pragma once
#include "SDL2/SDL.h"
#include "ErrorCode.hpp"

class GameObject;

class DisplayingSupporter
{
private:
    GameObject* m_associatedObject;
protected:
public:
    DisplayingSupporter(GameObject* obj);
    virtual ~DisplayingSupporter();
    virtual ErrorCode m_changeDisplayingDueTo(const SDL_Event& event); //The default behavior will be to do nothing.
    virtual bool m_isRelevantForDisplaying(const SDL_Event& event); //The default behavior will be to return yes if the game object was hit by a SDL_MOUSEBUTTONDOWN event or by a SDL_MOUSEMOTION event. Right clicks and other buttons are not being filtered out.
    virtual ErrorCode m_updateDisplaying(); //The default behavior will be to do nothing. The expected error codes are ErrorCode::NOTHING_HAPPENED and ErrorCode::MERELY_VISUAL_IMPLICATIONS. Not (!!!) ErrorCode::OK.
};

#endif