#include "ButtonSuggestor.hpp"

using ObjectData::Activeness;

ButtonSuggestor::ButtonSuggestor(Button* el): Suggestor(el), m_button(el)
{

}

ButtonSuggestor::~ButtonSuggestor()
{
std::cout<<"ButtonSuggestor destroyed for associated button: "<<m_button<<", scale value: "<<((int)(m_button->scaleValue))<<std::endl;
}

Suggestor::Suggestion ButtonSuggestor::m_suggestForRelevantEvent(const SDL_Event& event){
    //Suggest getting active iff clicked
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        Suggestor::Suggestion suggestion;
        suggestion.activeness = std::make_unique<Activeness>();
        Activeness& activeness = *(suggestion.activeness);
        if(m_button->m_isHitBy(event.button.x, event.button.y)){
            activeness = (Activeness)((int) activeness | (int) Activeness::ACTIVE);
        }
        else 
            activeness = (Activeness)((int) activeness ^ (int) Activeness::ACTIVE);
        return suggestion;
    }
    return Suggestor::Suggestion::None();
}

bool ButtonSuggestor::m_isRelevantForSuggesting(const SDL_Event& event){
    return event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT;
}