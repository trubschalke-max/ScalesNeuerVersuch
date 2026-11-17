#ifndef DRAWINGOBJECT_H
#define DRAWINGOBJECT_H
#pragma once

#include "ErrorCode.hpp"
#include "SDL2/SDL.h"
#include <vector>
#include "SuggestSupport/Suggestor.hpp"

class DrawingObject{
public: 
    enum class EventAcceptance{NONE, ONE, MANY};
public:
    DrawingObject();
    virtual ~DrawingObject();

    virtual ErrorCode m_draw() = 0;
    virtual ErrorCode m_handleEvent(const SDL_Event& event, ErrorList whatCanIIgnore = ErrorHandling::standardErrorHierarchy()) = 0; //The default behavior is to return NOTHING_HAPPENED; or if events are accepted, to backwardsly iterate through the queue and stop when the first object has not returned NOTHING_HAPPENED.
    virtual ErrorCode m_renderPresent() = 0; //This should call SDL_RenderPresent on the associated renderer(s).
    virtual void m_setAcceptsEvents(EventAcceptance ea) = 0; //In the composite drawing, all contained objects should have their event acceptance set accordingly.
    virtual const Suggestor::Suggestion*m_retrieveSuggestion() = 0;
    virtual ErrorCode m_resetSuggestion() = 0; //The main values are OK if there was one; and NOTHING_HAPPENED if there was none.
    virtual ErrorCode m_handleValidSuggestion(const Suggestor::Suggestion& suggestion) = 0; //Redirects the suggestion to the drawings, in case it's a compositedrawing.
    virtual ErrorCode m_updateDisplayingEverywhere() = 0; //This will update the displaying of all contained drawings or game objects.

};

#endif