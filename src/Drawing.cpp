#include "Drawing.hpp"
#include "GameObject.hpp"
#include "ErrorCode.hpp"
#include<algorithm>
#include <string>
#include <iostream>
#include <vector>

typedef Drawing::EventAcceptance EvAc;

Drawing::Drawing(SDL_Renderer* renderer):m_renderer(renderer){}
Drawing::~Drawing(){}

ErrorCode Drawing::m_add(GameObject* newObject){
    auto it = std::find(m_queue.begin(), m_queue.end(), newObject);
    if(it != m_queue.end()){
        return ErrorCode::DUPLICATE_LIST_INSERTION;
    }
    it = std::find_if(m_queue.begin(), m_queue.end(), [newObject](GameObject* current){return current->m_Layer()>newObject->m_Layer();});
    m_queue.insert(it, newObject);
    return ErrorCode::OK; 
}

ErrorCode Drawing::m_draw(){
    ErrorCode whatToReturn = ErrorCode::OK;
    if(!m_renderer)
        return ErrorCode::SDL_RENDERER_NULL;
    for(GameObject* objectToDraw: m_queue){
        ErrorHandling::checkForErrorStandardHierarchy(objectToDraw->m_draw(m_renderer), whatToReturn);
    }
    return whatToReturn;
}

ErrorCode Drawing::m_handleEvent(const SDL_Event& event, ErrorList whatCanIIgnore){
    if(m_acceptsEvents == EvAc::NONE){
        return ErrorCode::DRAWING_MUST_NOT_HANDLE_EVENTS;
    }
    ErrorCode error = ErrorCode::NOTHING_HAPPENED;
    for(GameObject* object: m_queue){//Iterate through the queue in reverse in order to hit the latest drawn object! 
        ErrorCode rawEC = object->m_handleEvent(event);
        //Return on first error and, if that's the specification, on the first event
        if(rawEC == ErrorCode::NOT_ACTUALLY_CLICKED)
            rawEC = ErrorCode::NOTHING_HAPPENED;
        ErrorHandling::checkForErrorStandardHierarchy(rawEC, error);
        if(m_acceptsEvents == EvAc::ONE && rawEC == ErrorCode::OK || std::find(whatCanIIgnore.begin(), whatCanIIgnore.end(), rawEC) == whatCanIIgnore.end()){ //Return on first error and possibly on the first successful event handling. If nothing happened, we iterate further.
            return error;
        }
    }
    return error;
}

ErrorCode Drawing::m_renderPresent(){
    if(!m_renderer)
        return ErrorCode::SDL_RENDERER_NULL;
    SDL_RenderPresent(m_renderer);
    return ErrorCode::OK;
}

const Suggestor::Suggestion* Drawing::m_retrieveSuggestion(){
    for(GameObject* object: m_queue){
        const Suggestor::Suggestion& suggestion = object->m_CurrentSuggestion();
        if(!suggestion.m_empty()){
            return &suggestion;
        }
    }
    return nullptr;
}

ErrorCode Drawing::m_remove(GameObject* objectToRemove){
    if(!objectToRemove)
        return ErrorCode::NULLPOINTER_EXCEPTION;
    auto it = std::find(m_queue.begin(), m_queue.end(), objectToRemove);
    if(it == m_queue.end()){
        return ErrorCode::OBJECT_NOT_IN_QUEUE;
    }
    m_queue.erase(it);
    return ErrorCode::OK;
}

ErrorCode Drawing::m_resetSuggestion(){
    ErrorCode error = ErrorCode::OK;
    for(GameObject* object: m_queue){
        ErrorHandling::checkForErrorStandardHierarchy(object->m_resetSuggestion(), error);
    }
    return error;
}

ErrorCode Drawing::m_handleValidSuggestion(const Suggestor::Suggestion& suggestion){
    bool redraw = false;
    ErrorCode error = ErrorCode::NOTHING_HAPPENED;
    int okCounter = 0;
    for(GameObject* object: m_queue){
        ErrorCode rawEC;
        switch(object->m_HandlingMode()){
            case GameObject::HandlingMode::SUGGEST:
                if(object->m_isRelevant(suggestion))
                    rawEC = object->m_putIntoEffect(suggestion);
                else rawEC = ErrorCode::NOTHING_HAPPENED;
                break;
            case GameObject::HandlingMode::DISPLAY_ONLY:
                rawEC = object->m_updateDisplaying();
                if(rawEC == ErrorCode::MERELY_VISUAL_IMPLICATIONS || rawEC == ErrorCode::NOTHING_HAPPENED){
                    rawEC = ErrorCode::NOTHING_HAPPENED;
                    redraw = true;
                }
                break;
            case GameObject::HandlingMode::DIRECT_HANDLING: //I'm not sure yet what to do then. It should rather not happen.
                throw std::logic_error("DIRECT_HANDLING mode should not be encountered in m_handleValidSuggestion.");
            default:
            throw std::logic_error("Unhandled handling mode encountered in m_handleValidSuggestion.");
                break;
        }
        if(rawEC == ErrorCode::OK){
            okCounter++;
            std::cout<< "Object handled successfully. OkCounter: " << okCounter << std::endl;
        }

        ErrorHandling::checkForErrorStandardHierarchy(rawEC, error);
    }
    if(error != ErrorCode::NOTHING_HAPPENED && error != ErrorCode::OK)
        return error;
    if(suggestion.applicability != Suggestor::Suggestion::ApplicabilityType::MULTIPLE && suggestion.applicability != Suggestor::Suggestion::ApplicabilityType::ERROR && okCounter > (int) suggestion.applicability){
        throw std::runtime_error("Multiple handlings of suggestion detected");
        error = ErrorCode::MULTIPLE_HANDLINGS_OF_SUGGESTION;
        return error;
    }
    if(okCounter == 0)
        error = ErrorCode::NOTHING_HAPPENED; //This should not happen.
    if(redraw || okCounter > 0){
        ErrorCode drawError = ErrorCode::OK;
        ErrorHandling::checkForErrorStandardHierarchy(m_draw(), drawError);
        ErrorHandling::checkForErrorStandardHierarchy(m_renderPresent(), drawError);
        if(drawError != ErrorCode::OK)
            return drawError;
    }
    return error;
}

ErrorCode Drawing::m_updateDisplayingEverywhere(){
    ErrorCode error = ErrorCode::OK;
    for(GameObject* object: m_queue){
        ErrorHandling::checkForErrorStandardHierarchy(object->m_updateDisplaying(), error);
    }
    return error;
}