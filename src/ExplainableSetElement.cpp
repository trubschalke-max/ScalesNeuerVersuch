#include "ExplainableSetElement.hpp"
#include "HoverableCompositeButtonElement.hpp"
#include "ExplainableSet.hpp"
#include <algorithm>
#include "SuggestSupport/ExplainableSetElementSuggestor.hpp"

using ObjectData::ExplainableSetValue;
using ObjectData::Explainable;
using ObjectData::WordState;
using ObjectData::Activeness;

ExplainableSetElement::ExplainableSetElement(ObjectData::PlayerID playerID, int x, int y, ExplainableSet* explainables, bool forExplainingPlayer, Layer layer): Comp(playerID, x, y, layer), m_explainables(explainables), m_forExplainingPlayer(forExplainingPlayer)
{
    m_colors = s_standardColors();
  if(m_createButtons() != ErrorCode::OK){
        throw std::runtime_error("Unable to create buttons for scale!");
    }
    m_suggestor = std::make_unique<ExplainableSetElementSuggestor>(this);
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
            SDL_Color{235,235,190},
            SDL_Color{255, 150, 225},
            SDL_Color{150, 150, 255}
        }
    };
}

ExplainableSetValue ExplainableSetElement::m_valueAt(int x, int y){//This assumes that the buttons do not intersect.
    for(int i(0); i < m_buttons.size(); ++i){
        auto& button = m_buttons[i];
        if(button->m_isHitBy(x, y)){
            return (ExplainableSetValue) i;
        }
    }
    return ExplainableSetValue::EXSET_NONE;
}

ErrorCode ExplainableSetElement::m_createButtons(){
    const auto& explainables = m_explainables->m_Explainables();
    int buttonHeight = 30;
    int buttonWidth = 200;
    for(int i(0); i < explainables.size(); ++i){
        int maxYSteps = 8; //This should be an enum value or something, heavily related to some standard value of the game logic. For now, it is just a number.
        auto& explainable = explainables[i];
        SDL_Rect rect {m_x + (i/maxYSteps)*buttonWidth, m_y + (i % maxYSteps) * buttonHeight, buttonWidth, buttonHeight};
        ObjectData::Activeness activeness = m_figureOutActiveness(explainable.state);
        m_buttons.emplace_back(std::make_unique<HoverableButtonElement>(m_playerID, rect, explainable.word, m_colors.basicColors, m_colors.furtherColors, GameObject::DEFAULT_LAYER, activeness));
    }
    return ErrorCode::OK;
}

ObjectData::Activeness ExplainableSetElement::m_figureOutActiveness(ObjectData::WordState state){
    if(m_forExplainingPlayer && (int) state & (int) WordState::TRIED != 0){//An explaining player should not have to try.
        return ObjectData::Activeness::ACTIVENESS_ERROR; //The player should not be able to see the correct answer, and the player should not be able to see the wrong answer. 
    }
    switch(state){
        case WordState::WRONG:
                if(!m_forExplainingPlayer){
                    return ObjectData::Activeness::PASSIVE;
                } else {
                    return ObjectData::Activeness::SECRETLY_PASSIVE;
                }
        case WordState::CORRECT:
                if(!m_forExplainingPlayer){
                    return ObjectData::Activeness::PASSIVE;
                } else {
                    return ObjectData::Activeness::SECRETLY_ACTIVE;
                }        
        case WordState::WRONG_AND_TRIED:
            return ObjectData::Activeness::FORBIDDEN;
        case WordState::CORRECTLY_TRIED:
            return ObjectData::Activeness::PERMANENTLY_SELECTED; //Actually, the players try to guess the wrong words. In the future, I might want to implement a warning color for a losing player, but I don't have one yet. 
        default:
            return ObjectData::Activeness::ACTIVENESS_ERROR; //It does not yet make sense for all values.
    }
}

ErrorCode ExplainableSetElement::m_putIntoEffect(const Suggestor::Suggestion& suggestion){
    if(suggestion.explainables && suggestion.selectedWordValue){
        return ErrorCode::MULTIPLE_SUGGESTIONS; //This should not happen, because the suggestion should be either for resetting the word set as a whole, or for selecting a specific word, but not both;
    }
    if(suggestion.explainables){
        return m_handleExplainableSetSuggestion(*suggestion.explainables);
    }
    //if (suggestion.selectedWordValue) is handled below.
    ErrorCode error =m_handleWordSelectionSuggestion(*suggestion.selectedWordValue);
    if(error == ErrorCode::OK || error == ErrorCode::SUCCESS || error == ErrorCode::CORRECT_WORD_WAS_JUST_SELECTED){
        ErrorHandling::checkForErrorStandardHierarchy(m_refreshButtonsActiveness(), error); 
    }
    if(error == ErrorCode::INVALID_WORD_SELECTION || error == ErrorCode::WORD_ALREADY_TRIED || error == ErrorCode::CORRECT_WORD_ALREADY_SELECTED){
        return ErrorCode::NOTHING_HAPPENED;
    }
    std::for_each(m_explainables->m_Explainables().begin(), m_explainables->m_Explainables().end(), [](const Explainable& e){std::cout << "Word \"" << e.word << "\" has state " << toString(e.state) << std::endl;});
    return error;
}

ErrorCode ExplainableSetElement::m_refreshButtonsActiveness(){
    const auto& explainables = m_explainables->m_Explainables();
    if(explainables.size() != m_buttons.size()){
        throw std::logic_error("Inconsistent sizes between explainables and buttons"); //This should not happen, because the buttons are created based on the explainables, and the explainables should not change in size.
    }
    for(int i(0); i < 4; ++i){ //For testing/debugging purposes, I will only refresh the first four buttons. 
//     i < explainables.size(); ++i){
        auto& button = m_buttons[i];
        auto& explainable = explainables[i];
        int hoveredActiveness = ((int) explainable.state) & ((int) WordState::TRIED) == 0 ? (int) Activeness::HOVERED_OVER : 0;
        int newActiveness = (int) m_figureOutActiveness(explainable.state);
        button->m_setActiveness((Activeness)(newActiveness | hoveredActiveness)); //If the button is hovered over, it should remain hovered over, even if the activeness changes.
    }
    return ErrorCode::OK;
}

ErrorCode ExplainableSetElement::m_handleExplainableSetSuggestion(const std::vector<Explainable>& suggestions){
    throw std::logic_error("Handling of explainable set suggestions is not yet implemented.");
}

ErrorCode ExplainableSetElement::m_handleWordSelectionSuggestion(ExplainableSetValue selectedValue){
    ErrorCode error = m_explainables->m_guess(selectedValue);
    if(error == ErrorCode::SUCCESS || error == ErrorCode::CORRECT_WORD_WAS_JUST_SELECTED){//This would lead to many implications for the game itself, but they are not yet implemented. For now, I will just refresh the buttons' activeness.
        return m_refreshButtonsActiveness();
    }
    return error;
}

