#ifndef SCALEELEMENTSUGGESTOR_H
#define SCALEELEMENTSUGGESTOR_H

#pragma once
#include "ScaleElement.hpp"

class ScaleElementSuggestor: public Suggestor
{

private:
    ScaleElement* m_scaleElement;
    Suggestor::Suggestion m_suggestForSelection(ObjectData::ScaleData oldData, ObjectData::ScaleValue selection);
public:
    ScaleElementSuggestor(ScaleElement* el);
    virtual ~ScaleElementSuggestor();

    bool m_isRelevantForSuggesting(const SDL_Event& event);
    Suggestor::Suggestion m_suggestForRelevantEvent(const SDL_Event& event);
};

#endif