#include "ExplainableSetElementSuggestor.hpp"
#include "ExplainableSetElement.hpp"

using ObjectData::ExplainableSetValue;

ExplainableSetElementSuggestor::ExplainableSetElementSuggestor(ExplainableSetElement* element): HoverableCompositeButtonElementSuggestor(element), m_element(element)
{

}

ExplainableSetElementSuggestor::~ExplainableSetElementSuggestor()
{

}

Suggestor::Suggestion ExplainableSetElementSuggestor::m_suggestForRelevantEvent(const SDL_Event& event){
    Suggestion toSuggest;
    if(event.type != SDL_MOUSEBUTTONDOWN){
        throw std::logic_error("Weird error: An ExplainableSetElementSuggestor suggests something for a non-mousebuttondown event");
    }
    if(!m_element->m_isHitBy(event.button.x, event.button.y)){
        throw std::logic_error("Weird error: An ExplainableSetElementSuggestor suggests something for a non-hitting mousebuttondown event");
    }
    if(event.button.button != SDL_BUTTON_LEFT){
        return toSuggest;
    }
    ExplainableSetValue selected = m_element->m_valueAt(event.button.x, event.button.y);
    toSuggest.selectedWordValue = std::make_unique<ExplainableSetValue>(selected);
    return toSuggest; //Right now, the explainable set element does not know if it is allowed to actually alter the explainables. Don't overestimate the meaning of a suggestion.
}

