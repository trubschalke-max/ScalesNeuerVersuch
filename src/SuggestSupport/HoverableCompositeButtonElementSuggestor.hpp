#ifndef HOVERABLECOMPOSITEBUTTONELEMENTSUGGESTOR_H
#define HOVERABLECOMPOSITEBUTTONELEMENTSUGGESTOR_H

#pragma once

#include "SuggestSupport/Suggestor.hpp"

class HoverableCompositeButtonElement;

class HoverableCompositeButtonElementSuggestor: public Suggestor
{

private:
    HoverableCompositeButtonElement* m_element;
public:
    HoverableCompositeButtonElementSuggestor(HoverableCompositeButtonElement* element);
    virtual ~HoverableCompositeButtonElementSuggestor();
    virtual Suggestor::Suggestion m_suggestForRelevantEvent(const SDL_Event& event) override = 0;
    virtual bool m_isRelevantForSuggesting(const SDL_Event& event) override;

};

#endif