#include "HoverableCompositeButtonElement.hpp"
#include "SuggestSupport/HoverableCompositeButtonElementSuggestor.hpp"

HoverableCompositeButtonElementSuggestor::HoverableCompositeButtonElementSuggestor(HoverableCompositeButtonElement* element): Suggestor(element), m_element(element)
{

}

HoverableCompositeButtonElementSuggestor::~HoverableCompositeButtonElementSuggestor()
{

}

bool HoverableCompositeButtonElementSuggestor::m_isRelevantForSuggesting(const SDL_Event& event){
    if(event.type != SDL_MOUSEBUTTONDOWN){
        return false;
    }
    if(!m_element->m_isHitBy(event.button.x, event.button.y)){
        return false;
    }
    if(event.button.button != SDL_BUTTON_LEFT)
        return false;
    return true; //Right now, the scale element does not know if it is allowed to actually alter the scale data. Don't overestimate the meaning of a suggestion.
}