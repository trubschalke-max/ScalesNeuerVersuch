#ifndef EXPLAINABLESETELEMENT_H
#define EXPLAINABLESETELEMENT_H
#pragma once

#include "GameObject.hpp"
#include "HoverableCompositeButtonElement.hpp"
#include "ExplainableSet.hpp"

class ExplainableSetElement: public Comp{//This class manages the displaying of a set of selectable words. 
private:
    bool m_forExplainingPlayer; 
    ExplainableSet* m_explainables;
    ObjectData::ExplainableSetValue m_valueAt(int x, int y); //This function recognizes the buttons. If the value does not belong to a button, EXSET_NONE is being returned.
    ErrorCode m_handleExplainableSetSuggestion(const std::vector<ObjectData::Explainable>& suggestions); // I don't yet know how to handle resetting the explainables pool. So I will throw an exception if it is called.
    ErrorCode m_handleWordSelectionSuggestion(ObjectData::ExplainableSetValue selectedValue); //This is a private function which assumes validity, and directs to ExplainableSet::m_setCorrectValue(). It does not refresh the buttons' activeness.
    ErrorCode m_refreshButtonsActiveness() override; //This function refreshes the activeness of the buttons based on the current state of the explainables. It is called after a suggestion has been put into effect, and also when the element is created.
    
public:
    ExplainableSetElement(ObjectData::PlayerID playerID, int x, int y, ExplainableSet* explainables, bool forExplainingPlayer = false, Layer layer = Layer::DEFAULT_LAYER);
    ~ExplainableSetElement();

    ErrorCode m_createButtons() override;
    ErrorCode m_putIntoEffect(const Suggestor::Suggestion& suggestion) override;
    ObjectData::Activeness m_figureOutActiveness(ObjectData::WordState state);

    static Comp::ColorSet s_standardColors();
    bool m_isRelevant(const Suggestor::Suggestion& suggestion) override;
    friend class ExplainableSetElementSuggestor;

    void m_setForExplainingPlayer(bool forExplainingPlayer){m_forExplainingPlayer = forExplainingPlayer;}
};

#endif