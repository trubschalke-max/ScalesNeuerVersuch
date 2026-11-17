#include "SuggestSupport/DisplayingSupporter.hpp"
#include "GameObject.hpp"
#include <iostream>

DisplayingSupporter::DisplayingSupporter(GameObject* obj): m_associatedObject(obj)
{

}

DisplayingSupporter::~DisplayingSupporter()
{
    std::cout << "DisplayingSupporter destroyed for associated object: " << m_associatedObject << std::endl;
}

ErrorCode DisplayingSupporter::m_changeDisplayingDueTo(const SDL_Event& event){
    return ErrorCode::NOTHING_HAPPENED;
}

bool DisplayingSupporter::m_isRelevantForDisplaying(const SDL_Event& event){
    switch(event.type){
        case SDL_MOUSEMOTION: return m_associatedObject->m_isHitBy(event.motion.x, event.motion.y); //I assume that the even handlers and drawings have avoided events from the wrong windows.  
        case SDL_MOUSEBUTTONDOWN: return m_associatedObject->m_isHitBy(event.button.x, event.button.y);
        default: return false;
    }
}

ErrorCode DisplayingSupporter::m_updateDisplaying(){
    return ErrorCode::NOTHING_HAPPENED;
}