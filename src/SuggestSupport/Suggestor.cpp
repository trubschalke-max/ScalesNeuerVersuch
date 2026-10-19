#include "SuggestSupport/Suggestor.hpp"
#include <iostream>

ErrorCode Suggestor::Suggestion::m_validate() const{
    int count = 0;
    if(certainlyKnownError != ErrorCode::OK){
        return certainlyKnownError;
    }
    if(m_empty()){
        return ErrorCode::EMPTY_SUGGESTION;
    }
    if(activeness){
        count++;
        if(!buttonElement){
            return ErrorCode::EMPTY_BUTTON_ELEMENT_INSIDE_THE_SUGGESTION;
        }
    }
    if(scaleData){
        count++;
    }
    if(explainables){
        count++;
    }
    if(selectedWordValue){
        count++;
    }
    if(count > 1){
        return ErrorCode::MULTIPLE_SUGGESTION_ITEMS_AT_ONCE;
    }
    return ErrorCode::OK;
}

Suggestor::Suggestor(GameObject* obj): m_associatedObject(obj)
{
}

Suggestor::~Suggestor()
{
    std::cout << "Suggestor destroyed for associated object: " << m_associatedObject << std::endl;

}

bool Suggestor::Suggestion::m_empty() const{return activeness.get() == nullptr && scaleData.get() == nullptr && explainables.get() == nullptr && selectedWordValue.get() == nullptr && buttonElement == nullptr && certainlyKnownError == ErrorCode::OK;}

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
    sug.explainables.reset(nullptr);
    sug.selectedWordValue.reset(nullptr);
    sug.buttonElement = nullptr;
    sug.certainlyKnownError = ErrorCode::OK;
    return sug;
}