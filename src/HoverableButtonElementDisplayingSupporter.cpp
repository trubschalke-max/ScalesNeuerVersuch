#include "HoverableButtonElementDisplayingSupporter.hpp"

typedef HoverableButtonElementDisplayingSupporter Supp;
using ObjectData::Activeness;

Supp::HoverableButtonElementDisplayingSupporter(HoverableButtonElement* button): DisplayingSupporter(button), m_button(button)
{
    
}

Supp::~HoverableButtonElementDisplayingSupporter()
{
    std::cout << "Destroying HoverableButtonElementDisplayingSupporter" << std::endl;
}

ErrorCode Supp::m_changeDisplayingDueTo(const SDL_Event& event){
    if(event.type != SDL_MOUSEMOTION)
        return ErrorCode::NOTHING_HAPPENED;
    int oldActiveness = (int) m_button->m_Activeness(), newActiveness = oldActiveness;
    if(m_button->m_isHitBy(event.motion.x, event.motion.y)){
        newActiveness |= (int) Activeness::HOVERED_OVER;
    }
    else newActiveness &=~( (int) Activeness::HOVERED_OVER);
    m_button->m_setActiveness((Activeness)newActiveness);
    if(newActiveness == oldActiveness)
        return ErrorCode::NOTHING_HAPPENED;
    else return ErrorCode::MERELY_VISUAL_IMPLICATIONS;
}

bool Supp::m_isRelevantForDisplaying(const SDL_Event& event){
    if(event.type != SDL_MOUSEMOTION) //Of course, I could create derived classes that, e.g., react to right clicks.
        return false;
    if(event.type == SDL_MOUSEMOTION){//SDL_MOUSEMOTION = 1024
        if((int)(m_button->m_Activeness()) & (int)Activeness::FORBIDDEN == (int)Activeness::FORBIDDEN){
            return false;
        }
    }
    return true;
}

