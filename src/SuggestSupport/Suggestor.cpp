#include "SuggestSupport/Suggestor.hpp"
#include <iostream>

Suggestor::Suggestor(GameObject* obj): m_associatedObject(obj)
{
}

Suggestor::~Suggestor()
{
    std::cout << "Suggestor destroyed for associated object: " << m_associatedObject << std::endl;

}

bool Suggestor::Suggestion::m_empty() const{return activeness.get() == nullptr && scaleData.get() == nullptr && explainables.get() == nullptr;}

Suggestor::Suggestion Suggestor::m_suggestFor(const SDL_Event& event){
    if(!m_isRelevantForSuggesting(event)){
        throw std::runtime_error("Suggesting an irrelevant event");
    }
    return m_suggestForRelevantEvent(event);
}

bool Suggestor::m_isRelevantForSuggesting(const SDL_Event& event){
    return false;
}

Suggestor::Suggestion Suggestor::m_suggestForRelevantEvent(const SDL_Event& event){
    Suggestion sug;
    sug.activeness.reset(nullptr);
    sug.scaleData.reset(nullptr);
    return sug;
}