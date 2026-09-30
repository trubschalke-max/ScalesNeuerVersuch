#include "ScaleElement.hpp"
#include "Graphics.hpp"

typedef HoverableButtonElement ScaleLike;

ScaleElement::ScaleElement(int x, int y, Scale* scale): GameObject(x,y), m_colors(s_standardColors()), m_associatedScale(scale)
{
    if(m_createButtons() != ErrorCode::OK){
        throw std::runtime_error("Unable to create buttons for scale!");
    }
    m_isClickable = true;
}

ScaleElement::~ScaleElement()
{

}

ErrorCode ScaleElement::m_createButtons(){
    auto scaleData = m_associatedScale->m_Data();
    Svalue minValue = scaleData.minValue, maxValue = scaleData.maxValue;
    int imin = (int) minValue, imax = (int) maxValue;
    ErrorCode whatToReturn = ErrorCode::OK;

    for(int i = imax; i >= imin - FURTHER_SCALE_BUTTONS_COUNT; --i){ //Create one button at a time
        int currentY = (imax - i) * m_HeightPerElement() + m_y;
        std::string currentText = m_associatedScale->m_textAt((Svalue)(i));

        SDL_Rect rect {m_x, currentY, m_Width(), m_HeightPerElement()};
        ButtonElement::Activeness activeness = m_associatedScale->m_Data().selection == (Svalue) i ? ButtonElement::ACTIVE : ButtonElement::PASSIVE;
        ScaleLike button(rect, currentText, m_colors.basicColors, m_colors.furtherColors, GameObject::DEFAULT_LAYER, activeness);
        m_buttons.push_back(button);
    }    
    if(m_buttons.size() != imax - imin + FURTHER_SCALE_BUTTONS_COUNT + 1){
        throw std::runtime_error("Wrong size of buttons vector");
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
        HoverableButtonElement::ColorSetExtension{
            SDL_Color{170, 255, 100},
            SDL_Color{210, 210, 150},
            SDL_Color{255,255,150},
        }
    };
}

ErrorCode ScaleElement::m_draw(SDL_Renderer* renderer){
    ErrorCode error = ErrorCode::OK;
    for(auto& button: m_buttons){
        error = button.m_draw(renderer);
        if(error != ErrorCode::OK){
        throw std::runtime_error("Drawing a scale has failed!");
        }
    }
    return error;
}

bool ScaleElement::m_isHitBy(int x, int y){
    for(auto& button: m_buttons){
        if(button.m_isHitBy(x,y)){
            return true;
        }
    }
    return false;
}

ErrorCode ScaleElement::m_leftClick(int x, int y){//Later on, the scale element will be configured in a way such that they obey the rules of the game.
    bool done = false;
    bool remakeButtons = false;
    ErrorCode error = ErrorCode::OK; //When at the end, it is still okay, the selection, the commitment or the undoing was successful.
    Svalue selection = m_valueAt(x,y);
    if(selection == m_associatedScale->m_scaleValueForCommitting()){
        error = m_associatedScale->m_commit();
        done = true;
    }
    if(selection == m_associatedScale->m_scaleValueForUndoing()){
        error = m_associatedScale->m_undo();
        done = true;
    }
    if(m_associatedScale->m_contains(selection)){
        error = m_associatedScale->m_select(selection);
        done = true;
    }
    if(done == false){
        throw std::runtime_error("A weird scale value was tried to select");
    }
    if(error == ErrorCode::SUCCESS){
        error = m_refreshButtonsActiveness();
    }
    if(error == ErrorCode::ALREADY_COMMITTED)
        return ErrorCode::NOTHING_HAPPENED;
    if(error == ErrorCode::INVALID_SCALE_SELECTION)
        return ErrorCode::NOTHING_HAPPENED;
    if(error == ErrorCode::INVALID_REDOING)
        return ErrorCode::NOTHING_HAPPENED;
    if(error == ErrorCode::INVALID_COMMIT)
        return ErrorCode::NOTHING_HAPPENED;
    return error;
}

Scale::ScaleValue ScaleElement::m_valueAt(int x, int y){
    if(!m_isHitBy(x,y)){
        throw std::runtime_error("ScaleElement::valueAt() is being called without the ScaleElement having been clicked on");
    }
    int imax = (int) m_associatedScale->m_Data().maxValue,
    imin = (int) m_associatedScale->m_Data().minValue,
    height = m_HeightPerElement();

    int apparentValue = imax - (y-m_y) / height;
    if(apparentValue < imin - FURTHER_SCALE_BUTTONS_COUNT || apparentValue > imax){
        return Scale::DUMMY_VALUE;
    }
    return (Scale::ScaleValue) apparentValue;
}

ErrorCode ScaleElement::m_refreshButtonsActiveness(){
try{
    auto data = m_associatedScale->m_Data();
    for(int i(0); i < m_buttons.size(); ++i){
        Scale::ScaleValue currentValue = (Scale::ScaleValue) (data.maxValue - i);
        m_buttons[i].m_setActiveness(m_figureOutActiveness(currentValue));
    }
    return ErrorCode::OK;
}
catch(InvalidSelection& is){
    return ErrorCode::SCALE_BUTTON_REFRESHING_FAIL;
}
}

int ScaleElement::m_buttonIndexFor(Scale::ScaleValue val){
    int imax = (int) m_associatedScale->m_Data().maxValue;
    int imin = (int) m_associatedScale->m_Data().minValue;

    if(val < imin - FURTHER_SCALE_BUTTONS_COUNT || val > imax){
        throw InvalidSelection(val);
    }
    int apparentReturnValue = imax - val;
    if(apparentReturnValue < 0 || apparentReturnValue >= m_buttons.size()){
        throw std::runtime_error("Weird button index");
    }
    return apparentReturnValue;
}

typedef ButtonElement::Activeness BA;

BA ScaleElement::m_figureOutActiveness(Scale::ScaleValue whatFor){
    auto data = m_associatedScale->m_Data();
    switch(data.state){
        case Scale::COMMITTED: 
            if(whatFor == m_associatedScale->m_scaleValueForCommitting())
                return BA::PERMANENTLY_SELECTED;
            if(whatFor == m_associatedScale->m_scaleValueForUndoing())
                return BA::FORBIDDEN;
            if(whatFor == data.selection)
                return BA::PERMANENTLY_SELECTED;
            return BA::FORBIDDEN;
        case Scale::NOT_YET_COMMITTED:
            if(data.selection == whatFor){
                switch(m_buttons[m_buttonIndexFor(whatFor)].m_Activeness()){
                    case BA::HOVERED_OVER_AND_ACTIVE: case BA::HOVERED_OVER_AND_PASSIVE:
                        return BA::HOVERED_OVER_AND_ACTIVE;
                    case BA::ACTIVENESS_ERROR: throw std::runtime_error("Some kind of activeness error!");
                    default: return BA::ACTIVE;
                }
            }
            else{// if wdata.selection != whatFor
                switch(m_buttons[m_buttonIndexFor(whatFor)].m_Activeness()){
                    case BA::HOVERED_OVER_AND_ACTIVE: case BA::HOVERED_OVER_AND_PASSIVE:
                        return BA::HOVERED_OVER_AND_PASSIVE;
                    case BA::ACTIVENESS_ERROR: throw std::runtime_error("Some kind of activeness/passiveness error!");
                    default: return BA::PASSIVE;
                }
            }
        default: return ButtonElement::Activeness::ACTIVENESS_ERROR;
    }
}