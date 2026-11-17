#ifndef GUESSINGDATACREATOR_H
#define GUESSINGDATACREATOR_H
#pragma once

#include "ScaleSet.hpp"
#include "ExplainableSet.hpp"

class GuessingDataCreator
{
private:
    ScaleSet::ScaleSetDataRecipe m_scaleSetDataRecipe;
public:
    GuessingDataCreator(ScaleSet::ScaleSetDataRecipe scaleSetDataRecipe);
    ~GuessingDataCreator();
    std::unique_ptr<ExplainableSet> m_createExplainableSet(); //This uses the explainable set data recipe to create an explainable set.
    std::unique_ptr<ScaleSet> m_createScaleSet(); //This uses the scale set data recipe to create a scale set.

};

#endif