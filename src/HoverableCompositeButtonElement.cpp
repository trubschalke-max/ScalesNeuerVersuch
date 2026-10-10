#include "HoverableCompositeButtonElement.hpp"

HoverableCompositeButtonElement::HoverableCompositeButtonElement(int x, int y, Layer layer): GameObject(x, y, layer)
{
    m_displayingSupporter = std::make_unique<HoverableCompositeButtonElementDisplayingSupporter>(this);
}

HoverableCompositeButtonElement::~HoverableCompositeButtonElement()
{

}

bool Comp::m_isHitBy(int x, int y){
    for(auto& button: m_buttons){
        if(button->m_isHitBy(x,y)){
            return true;
        }
    }
    return false;
}