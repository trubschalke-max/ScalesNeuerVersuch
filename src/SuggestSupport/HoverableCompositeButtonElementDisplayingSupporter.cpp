#include "SuggestSupport/HoverableCompositeButtonElementDisplayingSupporter.hpp"
#include <algorithm>
#include "HoverableCompositeButtonElement.hpp"

HoverableCompositeButtonElementDisplayingSupporter::HoverableCompositeButtonElementDisplayingSupporter(Comp* el): DisplayingSupporter(el), m_element(el)
#include "SuggestSupport/HoverableCompositeButtonElementDisplayingSupporter.hpp"

HoverableCompositeButtonElementDisplayingSupporter::HoverableCompositeButtonElementDisplayingSupporter()
{

}

HoverableCompositeButtonElementDisplayingSupporter::~HoverableCompositeButtonElementDisplayingSupporter()
{

}

bool CompSupp::m_isRelevantForDisplaying(const SDL_Event& event){//This assumes that the buttons do not intersect, and that any event handled by a suggestion machinery will not be included here.
    auto& buttons = m_element->m_buttons;
    if(std::find_if(buttons.begin(), buttons.end(), [&event](auto& el){
        return el->m_relevanceOf(event) == GameObject::RELEVANT;
    }) != buttons.end())
        return false;
    if(std::find_if(buttons.begin(), buttons.end(), [&event](auto& el){
        return el->m_relevanceOf(event) == GameObject::RELEVANT_FOR_DISPLAYING;
    }) != buttons.end())
        return true;
    return false;
}

ErrorCode CompSupp::m_changeDisplayingDueTo(const SDL_Event& event){
    ErrorCode error = ErrorCode::NOTHING_HAPPENED; 
    for(auto& button: m_element->m_buttons){
        if(button->m_relevanceOf(event) == GameObject::RELEVANT_FOR_DISPLAYING){
            ErrorCode rawEC = button->m_changeDisplayingDueTo(event);
            if(rawEC == ErrorCode::NOT_ACTUALLY_CLICKED)
                rawEC = ErrorCode::NOTHING_HAPPENED;
            ErrorHandling::checkForErrorStandardHierarchy(rawEC, error);
        }    
    }
    return error;
}


