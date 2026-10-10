#ifndef EXPLAINABLESET_H
#define EXPLAINABLESET_H
#pragma once

#include "GameObject.hpp"
#include "HoverableCompositeButtonElement.hpp"
#include "ExplainableSet.hpp"

class ExplainableSetElement: public Comp{//This class manages the displaying of a set of selectable words. 
private:
    ExplainableSet* m_explainables;
public:
    ExplainableSetElement(int x, int y, ExplainableSet* explainables, Layer layer = Layer::DEFAULT_LAYER);
    ~ExplainableSetElement();

    static Comp::ColorSet s_standardColors();

};

#endif