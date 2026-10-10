#include "SuggestSupport/ScaleElementSuggestor.hpp"

using ObjectData::ScaleData;
using ObjectData::ScaleValue;
using ObjectData::State;

ScaleElementSuggestor::ScaleElementSuggestor(ScaleElement* element): HoverableCompositeButtonElementSuggestor(element), m_scaleElement(element)
{

}

ScaleElementSuggestor::~ScaleElementSuggestor()
{

}


Suggestor::Suggestion ScaleElementSuggestor::m_suggestForRelevantEvent(const SDL_Event& event){
    Suggestion toSuggest;
    if(event.type != SDL_MOUSEBUTTONDOWN){
        throw std::logic_error("Weird error: A ScaleElementSuggestor suggests something for a non-mousebuttondown event");
    }
    if(!m_scaleElement->m_isHitBy(event.button.x, event.button.y)){
        throw std::logic_error("Weird error: A ScaleElementSuggestor suggests something for a non-hitting mousebuttondown event");
    }
    if(event.button.button != SDL_BUTTON_LEFT){
        return toSuggest;
    }
    if(m_scaleElement->m_associatedScale->m_Data().state != State::NOT_YET_COMMITTED)
        return toSuggest;
    ScaleData data = m_scaleElement->m_associatedScale->m_Data();
    ObjectData::ScaleValue selected = m_scaleElement->m_valueAt(event.button.x, event.button.y);
    return m_suggestForSelection(data, selected);
}

Suggestor::Suggestion ScaleElementSuggestor::m_suggestForSelection(ScaleData oldData, ObjectData::ScaleValue selection){
    Scale* scale = m_scaleElement->m_associatedScale;
    ObjectData::ScaleData newData = oldData;
    if(selection == scale->m_scaleValueForCommitting()){
        newData.selection = Svalue::SCALE_COMMIT;
    }
    if(selection == scale->m_scaleValueForUndoing()){
        newData.selection = Svalue::SCALE_UNDO;
    }
    if(scale->m_contains(selection)){
        newData.selection = selection;
    }
    return Suggestion{nullptr, std::make_unique<ObjectData::ScaleData>(newData)};
}
