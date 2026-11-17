#ifndef EXPLAINABLESETELEMENTSUGGESTOR_H
#define EXPLAINABLESETELEMENTSUGGESTOR_H
#pragma once

#include "SuggestSupport/Suggestor.hpp"
#include "SuggestSupport/HoverableCompositeButtonElementSuggestor.hpp"

class ExplainableSetElement;

class ExplainableSetElementSuggestor: public HoverableCompositeButtonElementSuggestor
{

private:
ExplainableSetElement* m_element;
public:
    ExplainableSetElementSuggestor(ExplainableSetElement* element);
    virtual ~ExplainableSetElementSuggestor();
    Suggestor::Suggestion m_suggestForRelevantEvent(const SDL_Event& event) override; 
};

typedef ExplainableSetElementSuggestor ExpSugg;

#endif