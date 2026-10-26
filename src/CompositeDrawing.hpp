#ifndef COMPOSITEDRAWING_H
#define COMPOSITEDRAWING_H
#pragma once

#include <vector>
#include <map>
#include "ErrorCode.hpp"
#include "SDL2/SDL.h"
#include "DrawingObject.hpp"

class Drawing;
class GameObject;
class CompositeDrawing: public DrawingObject//Possibly, this class could also contain DrawingObjects, but I don't need to.
{
private:
    std::map<int, Drawing*> m_drawings; //Those ints are actually playerIDs, but the CompositeDrawing class doesn't need to know about ObjectData or PlayerID directly.
public:
    CompositeDrawing();
    ~CompositeDrawing();
    ErrorCode m_draw(std::vector<int>&);
    ErrorCode m_draw();
    ErrorCode m_add(Drawing*, int);
    ErrorCode m_remove(Drawing*, int);// I assume that this will never be needed
    ErrorCode m_handleEvent(const SDL_Event& event, ErrorList whatCanIIgnore = ErrorHandling::standardErrorHierarchy()); //The default behavior is to return NOTHING_HAPPENED; or if events are accepted, to backwardsly iterate through the queue and stop when the first object has not returned NOTHING_HAPPENED.
    ErrorCode m_renderPresent() override;
    virtual void m_setAcceptsEvents(EventAcceptance ea) override; //In the composite drawing, all contained objects should have their event acceptance set accordingly.
    const Suggestor::Suggestion*m_retrieveSuggestion() override;
    ErrorCode m_resetSuggestion() override; //The main values are OK if there was one; and NOTHING_HAPPENED if there was none.
    ErrorCode m_handleValidSuggestion(const Suggestor::Suggestion& suggestion) override;
    ErrorCode m_updateDisplayingEverywhere() override; //This will update the displaying of all contained drawings.
};



#endif