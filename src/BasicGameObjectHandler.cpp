#include "BasicGameObjectHandler.hpp"
#include <iostream>
#include "GameObject.hpp"
#include <algorithm>

BasicGameObjectHandler::BasicGameObjectHandler(DrawingObject* whatToDraw): EventHandler(whatToDraw)
{
    if(!whatToDraw){
        throw std::runtime_error("null drawing for game object event handler!");
    }
    if(!whatToDraw){
        throw std::runtime_error("empty drawing for game object event handler!");
    }
}

BasicGameObjectHandler::~BasicGameObjectHandler()
{

}

EventHandler::HandlingResult BasicGameObjectHandler::m_handleRelevantEvent(const SDL_Event& event, EventHandler::HandlingClass diagnosis){//Diagnosis doesn't matter here
    ErrorCode error = m_whatToDraw->m_handleEvent(event);
    switch(error){
        case ErrorCode::OK:
            return HandlingResult::QUIT_AND_REDRAW;
        case ErrorCode::MERELY_VISUAL_IMPLICATIONS:
            return HandlingResult::REDRAW;
        case ErrorCode::NOTHING_HAPPENED: 
            return HandlingResult::GO_ON;
        default: std::cout<<"Handling result failure due to error code"<<error<<std::endl;
        return HandlingResult::FAILURE;
    }
}

ErrorCode BasicGameObjectHandler::m_enter(){
    m_whatToDraw->m_setAcceptsEvents(Drawing::EventAcceptance::ONE);
    return BasicEventHandler::m_enter();
}

EventHandler::HandlingClass BasicGameObjectHandler::m_classify(const SDL_Event& event){
    if(std::find(GameObject::s_sdlEventTypesRelevantForGameObjects().begin(), GameObject::s_sdlEventTypesRelevantForGameObjects().end(), event.type) != GameObject::s_sdlEventTypesRelevantForGameObjects().end())
        return UNSPECIFIED;
    return IRRELEVANT;
}

const Suggestor::Suggestion*BasicGameObjectHandler::m_retrieveSuggestion(){
    return m_whatToDraw->m_retrieveSuggestion();
}

ErrorCode BasicGameObjectHandler::m_resetSuggestion(){
    return m_whatToDraw->m_resetSuggestion();
}