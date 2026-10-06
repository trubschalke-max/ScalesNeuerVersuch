#ifndef BUTTONSUGGESTOR_H
#define BUTTONSUGGESTOR_H
#pragma once

#include "ClassicUnhoverableButtonElement.hpp"

class ButtonSuggestor: public Suggestor
{
Button* m_button;
public:
    ButtonSuggestor(Button* el);
    virtual ~ButtonSuggestor();

    bool m_isRelevantForSuggesting(const SDL_Event& event);
    Suggestor::Suggestion m_suggestForRelevantEvent(const SDL_Event& event);
};

#endif