#include "ScaleElement.hpp"
#include "Graphics.hpp"

typedef HoverableButtonElement ScaleLike;

ScaleElement::ScaleElement(int x, int y, Scale* scale): GameObject(x,y), m_colors(s_standardColors()), m_associatedScale(scale)
{
    m_associatedError = m_createButtons();
}

ScaleElement::~ScaleElement()
{

}

ErrorCode ScaleElement::m_createButtons(){
    auto scaleData = m_associatedScale->m_Data();
    Svalue minValue = scaleData.minValue, maxValue = scaleData.maxValue;
    int imin = (int) minValue, imax = (int) maxValue;
    ErrorCode whatToReturn = ErrorCode::OK;

    for(int i = imax; i >= imin; --i){ //Create one button at a time
        int currentY = (i - imin) * m_HeightPerElement() + m_y;
        std::string currentText = m_associatedScale->m_textAt((Svalue)(i));

        SDL_Rect rect {m_x, currentY, m_Width(), m_HeightPerElement()};
        ScaleLike button(rect, currentText, m_colors.basicColors, m_colors.colorWhenHoveredOver);
        m_buttons.push_back(button);
    }    
    if(m_buttons.size() != imax - imin + 1){
        whatToReturn = ErrorCode::WRONG_SIZE_OF_BUTTONS_VECTOR;
    }
    return whatToReturn;
}

ScaleElement::ColorSet ScaleElement::s_standardColors(){
    return ScaleElement::ColorSet{
            ClassicUnhoverableButtonElement::ColorSet{
                Graphics::yellowColor(),
                Graphics::shieldGreyColor(),
                Graphics::brownColor(),
        },
        Graphics::greenColor()
    };
}

ErrorCode ScaleElement::m_draw(SDL_Renderer* renderer){
    ErrorCode whatToReturn = ErrorCode::OK;
    for(auto& button: m_buttons){
        whatToReturn = button.m_draw(renderer);
        if(whatToReturn != ErrorCode::OK){
        throw std::runtime_error("Drawing a scale has failed!");
        }
    }
}

bool ScaleElement::m_isHitBy(int x, int y){
    for(auto& button: m_buttons){
        if(button.m_isHitBy(x,y)){
            return true;
        }
        else{
            return false;
        }
    }
}

