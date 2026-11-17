#include "PlayerStateIndicatingElementDisplayingSupporter.hpp"
#include "PlayerStateIndicatingElement.hpp"
#include <stdexcept>

PlayerStateIndicatingElementDisplayingSupporter::PlayerStateIndicatingElementDisplayingSupporter(PlayerStateIndicatingElement* element)
    : HoverableCompositeButtonElementDisplayingSupporter(element), m_playerStateIndicatingElement(element) 
{

}

PlayerStateIndicatingElementDisplayingSupporter::~PlayerStateIndicatingElementDisplayingSupporter()
{

}

ErrorCode PlayerStateIndicatingElementDisplayingSupporter::m_updateDisplaying(){
    if(!m_playerStateIndicatingElement)
        throw std::runtime_error("PlayerStateIndicatingElement is null");
    auto texts = m_playerStateIndicatingElement->m_generateButtonTexts();
    for(const auto& pair: texts){
        auto buttonType = pair.first;
        auto text = pair.second;
        if(!m_playerStateIndicatingElement->m_buttonMap.count(buttonType))
            throw std::runtime_error("Button not found for button type");
        auto button = m_playerStateIndicatingElement->m_buttonMap.at(buttonType);
        if(button)
            button->m_setText(text);
    }
    return ErrorCode::OK;
}