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
    ErrorCode whatToReturn;
    for(GameObject* objectToDraw: m_queue){
        whatToReturn = objectToDraw->m_draw(m_renderer);
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