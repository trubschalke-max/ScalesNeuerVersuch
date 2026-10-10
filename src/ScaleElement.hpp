#ifndef SCALEELEMENT_H
#define SCALEELEMENT_H

#pragma once

#include "Scale.hpp"
#include "HoverableButtonElement.hpp"
#include "HoverableCompositeButtonElement.hpp"
#include "SuggestSupport/HoverableButtonElementDisplayingSupporter.hpp"
#include <vector>

#define FURTHER_SCALE_BUTTONS_COUNT 2

class ScaleElement: public GameObject
{
private:
    Scale* m_associatedScale;
    ErrorCode m_createButtons();
    std::vector<std::unique_ptr<HoverableButtonElement>> m_buttons;
    ColorSet m_colors;
    int m_buttonIndexFor(ObjectData::ScaleValue val);
    ObjectData::Activeness m_figureOutActiveness(ObjectData::ScaleValue whatFor);
    ErrorCode m_handleScaleValueSuggestion(ObjectData::ScaleValue selectedValue); //This is a private function which assumes validity, and directs to Scale::m_select(), Scale::m_commit() or Scale::m_undo() as appropriate. It does not refresh the buttons' activeness.
protected:
    bool m_showHoveredState() override;

public:
    ScaleElement(int x, int y, Scale* scale);
    virtual ~ScaleElement();

    
    ObjectData::ScaleValue m_valueAt(int x, int y); //This function recognizes the commit button and the undo button. If the value does not belong to a button, DUMMY_VALUE is being returned.
    int m_Width(){return 200;} //Some value for later revisiting
    int m_HeightPerElement(){return 30;} //Some value for later revisiting
    ErrorCode m_refreshButtonsActiveness();
    virtual ErrorCode m_putIntoEffect(const Suggestor::Suggestion& suggestion);

    friend class ScaleElementDisplayingSupporter;
    friend class ScaleElementSuggestor;

    static ColorSet s_standardColors();
};

std::string toString (ObjectData::ScaleValue s);

class InvalidSelection: public std::exception{
public:
    ObjectData::ScaleValue s;
    InvalidSelection(ObjectData::ScaleValue ps): s(ps){}
    const char* what(){std::string str = "Invalid selection: " + toString(s); return str.c_str();}
};

#endif