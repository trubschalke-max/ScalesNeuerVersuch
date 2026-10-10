#ifndef SCALEELEMENTSUGGESTOR_H
#define SCALEELEMENTSUGGESTOR_H

#pragma once
#include "ScaleElement.hpp"
#include "SuggestSupport/HoverableCompositeButtonElementSuggestor.hpp"

class ScaleElementSuggestor: public HoverableCompositeButtonElementSuggestor
{

private:
    ScaleElement* m_scaleElement;
    Suggestor::Suggestion m_suggestForSelection(ObjectData::ScaleData oldData, ObjectData::ScaleValue selection);
public:
    ScaleElementSuggestor(ScaleElement* el);
    virtual ~ScaleElementSuggestor();
    Suggestor::Suggestion m_suggestForRelevantEvent(const SDL_Event& event) override;
};

#endif