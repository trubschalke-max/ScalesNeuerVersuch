#include "ScaleElement.hpp"
#include "Graphics.hpp"
#include "SuggestSupport/HoverableCompositeButtonElementDisplayingSupporter.hpp"
#include "SuggestSupport/ScaleElementSuggestor.hpp"
#include "HoverableCompositeButtonElement.hpp"

typedef HoverableButtonElement ScaleLike;
using ObjectData::Activeness;
using ObjectData::ScaleData;
using ObjectData::ScaleValue; 
using ObjectData::State;

ScaleElement::ScaleElement(int x, int y, Scale* scale): Comp(x,y), m_associatedScale(scale)
{
    m_colors = s_standardColors();
    if(m_createButtons() != ErrorCode::OK){
        throw std::runtime_error("Unable to create buttons for scale!");
    }
    m_suggestor = std::make_unique<ScaleElementSuggestor>(this);
    m_colors = s_standardColors();
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
       ObjectData::Activeness activeness = m_associatedScale->m_Data().selection == (Svalue) i ?Activeness::ACTIVE :Activeness::PASSIVE;
        m_buttons.emplace_back(std::make_unique<HoverableButtonElement>(rect, currentText, m_colors.basicColors, m_colors.furtherColors, GameObject::DEFAULT_LAYER, activeness));
        auto& currentButton = m_buttons.back();
        m_buttons.back()->scaleValue = (Svalue)(i);
    }    
    if(m_buttons.size() != imax - imin + FURTHER_SCALE_BUTTONS_COUNT + 1){
        throw std::runtime_error("Wrong size of buttons vector");
        whatToReturn = ErrorCode::WRONG_SIZE_OF_BUTTONS_VECTOR;
    }
    return whatToReturn;
}

ScaleElement::ColorSet ScaleElement::s_standardColors(){
    return Comp::ColorSet{
            ClassicUnhoverableButtonElement::ColorSet{
                Graphics::yellowColor(),
                Graphics::shieldGreyColor(),
                Graphics::brownColor(),
        },
        HoverableButtonElement::ColorSetExtension{
            SDL_Color{170, 255, 100},
            SDL_Color{210, 210, 150},
            SDL_Color{235,235,190},
            SDL_Color{255, 150, 225},
            SDL_Color{150, 150, 255}
        }
    };
}

/*
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
*/

ObjectData::ScaleValue ScaleElement::m_valueAt(int x, int y){
    int imax = (int) m_associatedScale->m_Data().maxValue,
    imin = (int) m_associatedScale->m_Data().minValue,
    height = m_HeightPerElement();

    int apparentValue = imax - (y-m_y) / height;
    if(apparentValue < imin - FURTHER_SCALE_BUTTONS_COUNT || apparentValue > imax){
        return ScaleValue::DUMMY_VALUE;
    }
    return (ObjectData::ScaleValue) apparentValue;
}

ErrorCode ScaleElement::m_refreshButtonsActiveness(){//This adjusts the activenesses based on the current scale data.
try{
    auto data = m_associatedScale->m_Data();
    for(int i(0); i < m_buttons.size(); ++i){
        ObjectData::ScaleValue currentValue = (ScaleValue) ((int)data.maxValue - i);
        m_buttons[i]->m_setActiveness(m_figureOutActiveness(currentValue));
    }
    return ErrorCode::OK;
}
catch(InvalidSelection& is){
    return ErrorCode::SCALE_BUTTON_REFRESHING_FAIL;
}
}

int ScaleElement::m_buttonIndexFor(ObjectData::ScaleValue val){
    int imax = (int) m_associatedScale->m_Data().maxValue;
    int imin = (int) m_associatedScale->m_Data().minValue;
    int ival = (int) val;

    if(ival < imin - FURTHER_SCALE_BUTTONS_COUNT || ival > imax){
        throw InvalidSelection(val);
    }
    int apparentReturnValue = imax - ival;
    if(apparentReturnValue < 0 || apparentReturnValue >= m_buttons.size()){
        throw std::runtime_error("Weird button index");
    }
    return apparentReturnValue;
}

typedef ObjectData::Activeness BA;

BA ScaleElement::m_figureOutActiveness(ObjectData::ScaleValue whatFor){
    auto data = m_associatedScale->m_Data();
    switch(data.state){
        case State::COMMITTED: 
            if(whatFor == m_associatedScale->m_scaleValueForCommitting())
                return BA::PERMANENTLY_SELECTED;
            if(whatFor == m_associatedScale->m_scaleValueForUndoing())
                return BA::FORBIDDEN;
            if(whatFor == data.selection)
                return BA::PERMANENTLY_SELECTED;
            else return BA::FORBIDDEN;
        case State::NOT_YET_COMMITTED:
            if(data.selection == whatFor){
                switch(m_buttons[m_buttonIndexFor(whatFor)]->m_Activeness()){
                    case BA::HOVERED_OVER_AND_ACTIVE: case BA::HOVERED_OVER_AND_PASSIVE:
                        return BA::HOVERED_OVER_AND_ACTIVE;
                    case BA::FORBIDDEN: case BA::PERMANENTLY_SELECTED: throw std::logic_error("Why would a button be forbidden or permanently selected without the scale being committed?");
                    case BA::ACTIVENESS_ERROR: default: throw std::runtime_error("Some kind of activeness error!");
                }
            }
            else{// if wdata.selection != whatFor
                switch(m_buttons[m_buttonIndexFor(whatFor)]->m_Activeness()){
                    case BA::HOVERED_OVER_AND_ACTIVE: case BA::HOVERED_OVER_AND_PASSIVE:
                        return BA::HOVERED_OVER_AND_PASSIVE;
                    case BA::ACTIVENESS_ERROR: throw std::runtime_error("Some kind of activeness/passiveness error!");
                    default: return BA::PASSIVE;
                }
            }
        default: return Activeness::ACTIVENESS_ERROR;
    }
}

ErrorCode ScaleElement::m_putIntoEffect(const Suggestor::Suggestion& suggestion){
    ErrorCode error = ErrorCode::NOTHING_HAPPENED;
    if(!suggestion.scaleData)
        return ErrorCode::NOTHING_HAPPENED;
    //Determine what actually changed

    if(suggestion.scaleData->coincidesWith(m_associatedScale->m_Data())){
        return ErrorCode::NOTHING_HAPPENED;
    }
    if(m_associatedScale->m_Data().state == State::COMMITTED){
        //If the scale was already committed, it shall not further be altered. 
        return ErrorCode::NOTHING_HAPPENED;
    }
    if(suggestion.scaleData->state == State::COMMITTED){//I do not intend to commit this way.
        throw std::logic_error("Commits should come from a scale value COMMIT");
        //ErrorHandling::checkForErrorStandardHierarchy(m_associatedScale->m_commit(), error); (Probably, I'll never need this line)
    }
    if(suggestion.scaleData->selection != m_associatedScale->m_Data().selection){
        ErrorHandling::checkForErrorStandardHierarchy(m_handleScaleValueSuggestion(suggestion.scaleData->selection), error);
    }
    //Refresh the buttons' activeness, if necessary
    switch(error){
        case ErrorCode::INVALID_REDOING:
        return ErrorCode::NOTHING_HAPPENED;
        case ErrorCode::INVALID_COMMIT:
        return ErrorCode::NOTHING_HAPPENED;
        case ErrorCode::INVALID_SCALE_SELECTION: //This really should not happen.
        throw std::logic_error("Something went wrong in ScaleElement::m_putIntoEffect(): The suggestion was to select an invalid scale value");
        case ErrorCode::SUCCESS:
        error = ErrorCode::OK;
        ErrorHandling::checkForErrorStandardHierarchy(m_refreshButtonsActiveness(), error);
        return error;
        default:
        throw std::logic_error("Something went wrong in ScaleElement::m_putIntoEffect(): Unexpected error code");
        return error;
    }
}

ErrorCode ScaleElement::m_handleScaleValueSuggestion(ObjectData::ScaleValue selectedValue){
    if(selectedValue == m_associatedScale->m_scaleValueForCommitting()){
        return m_associatedScale->m_commit();
    }
    if(selectedValue == m_associatedScale->m_scaleValueForUndoing()){
        return m_associatedScale->m_undo();
    }
    if(m_associatedScale->m_contains(selectedValue)){
        return m_associatedScale->m_select(selectedValue);
    }
    throw std::logic_error("Something went wrong in ScaleElement::m_handleScaleValueSuggestion(): The suggestion was to select an invalid scale value");
}

bool ScaleElement::m_showHoveredState(){
    return m_associatedScale->m_Data().state != State::COMMITTED;
}