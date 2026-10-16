#include "GameObject.hpp"
#include <iostream>

GameObject::GameObject(ObjectData::PlayerID playerID, int x, int y, Layer layer): m_playerID(playerID), m_x(x), m_y(y), m_layer(layer), m_suggestor(std::make_unique<Suggestor>(this)), m_displayingSupporter(std::make_unique<DisplayingSupporter>(this)){}
GameObject::~GameObject(){std::cout << "GameObject destroyed at (" << m_x << ", " << m_y << ")" << std::endl;}



ErrorCode GameObject::m_handleEvent(const SDL_Event& event){
    ErrorCode error = ErrorCode::OK;
    if(!m_currentSuggestion.m_empty())
        throw std::logic_error("First take care of the current suggestion!");
    HandlingMode usedHandlingMode = std::min((HandlingMode) m_mode, (HandlingMode) m_relevanceOf(event));
    switch(usedHandlingMode){
        case DIRECT_HANDLING: 
            m_currentSuggestion = m_suggestFor(event);
            checkForError(m_putIntoEffect(m_currentSuggestion), error);
            ErrorHandling::checkForErrorStandardHierarchy(m_resetSuggestion(), error);
            return error;
        case SUGGEST:
            m_currentSuggestion = m_suggestFor(event);
            if(m_currentSuggestion.m_empty())
                return ErrorCode::NOTHING_HAPPENED;
            else return ErrorCode::OK;
        case DISPLAY_ONLY:
            return m_changeDisplayingDueTo(event);
        case DO_NOTHING:
            return ErrorCode::NOTHING_HAPPENED;
        default: 
            throw std::logic_error("Bad relevance value when handling event in GameObject!");
    }

    //Freitag: Bitte an die unique_ptrs weiterleiten
}


GameObject::Relevance GameObject::m_relevanceOf(const SDL_Event& event){
    if(m_suggestor->m_isRelevantForSuggesting(event))
        return RELEVANT;
    if(m_displayingSupporter->m_isRelevantForDisplaying(event))
        return RELEVANT_FOR_DISPLAYING;
    return IRRELEVANT;
}

ErrorCode GameObject::m_changeDisplayingDueTo(const SDL_Event& event){
    return m_displayingSupporter->m_changeDisplayingDueTo(event);
}

Suggestor::Suggestion GameObject::m_suggestFor(const SDL_Event& event){
    auto suggestion = m_suggestor->m_suggestFor(event);
    if(!suggestion.m_empty())
        suggestion.playerID = m_playerID;
    return suggestion;
}

ErrorCode GameObject::m_putIntoEffect(const Suggestor::Suggestion& suggestion){
    if(suggestion.m_empty()){
        return ErrorCode::PUTTING_INTO_EFFECT_NOT_POSSIBLE;
    }
    throw std::logic_error("A default game object cannot put a nonempty suggestion into effect");
}

ErrorCode GameObject::m_resetSuggestion(){
    if(m_currentSuggestion.m_empty())
        return ErrorCode::NOTHING_HAPPENED;
    m_currentSuggestion = Suggestor::Suggestion::None();
    return ErrorCode::OK;
}

std::initializer_list<int> GameObject::s_sdlEventTypesRelevantForGameObjects(){
    return {SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP, SDL_MOUSEMOTION, SDL_KEYDOWN, SDL_KEYUP};
}