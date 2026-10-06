#include "BasicGameObjectHandler.hpp"
#include <iostream>


BasicGameObjectHandler::BasicGameObjectHandler(Drawing* whatToDraw): EventHandler(whatToDraw)
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
            SDL_RenderPresent(m_whatToDraw->m_Renderer());
            return HandlingResult::REDRAW; //This is provisoric. In the future, observing needs to be made. 
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