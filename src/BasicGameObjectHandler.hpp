#ifndef BASICGAMEOBJECTHANDLER_H
#define BASICGAMEOBJECTHANDLER_H

#pragma once

#include "EventHandler.hpp"
#include "SuggestSupport/Suggestor.hpp"
#include "DrawingObject.hpp"

class BasicGameObjectHandler: public EventHandler
{
protected:
    HandlingResult m_handleRelevantEvent(const SDL_Event& event, HandlingClass diagnosis) override; //This will mostly have the game objects make a suggestion.
    virtual HandlingClass m_classify(const SDL_Event& event) override;
    virtual ErrorCode m_enter() override;
    bool m_expectsSuggestion = false;

public:
    BasicGameObjectHandler(DrawingObject* whatToDraw);//
    virtual ~BasicGameObjectHandler(); 
    const Suggestor::Suggestion*m_retrieveSuggestion(); //Retrieves the suggestion made by the game objects. The suggestion needs to be unique. 
    bool m_ExpectsSuggestion() const { return m_expectsSuggestion; }
    ErrorCode m_resetSuggestion(); //The main values are OK if there was one; and NOTHING_HAPPENED if there was none.
    ErrorCode m_handleValidSuggestion(const Suggestor::Suggestion& suggestion); //Redirects the suggestion to the drawing.
    ErrorCode m_updateDisplaying(); //Updates the displaying of all contained game objects.
};

#endif