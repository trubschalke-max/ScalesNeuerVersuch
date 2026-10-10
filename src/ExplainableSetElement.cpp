#include "ExplainableSetElement.hpp"

ExplainableSetElement::ExplainableSetElement(int x, int y, ExplainableSet* explainables, Layer layer): Comp(x, y, layer)
    : m_explainables(explainables)
{

}

ExplainableSetElement::~ExplainableSetElement()
{

}

Comp::ColorSet ExplainableSetElement::s_standardColors(){
    return Comp::ColorSet{
            ClassicUnhoverableButtonElement::ColorSet{
                Graphics::yellowColor(),
                Graphics::shieldGreyColor(),
                Graphics::brownColor(),
        },
        HoverableButtonElement::ColorSetExtension{
            SDL_Color{170, 255, 100},
            SDL_Color{210, 210, 150},
            SDL_Color{255,255,150},
            SDL_Color{255, 150, 225}
        }
    };
}