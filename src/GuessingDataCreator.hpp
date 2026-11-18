#ifndef GUESSINGDATACREATOR_H
#define GUESSINGDATACREATOR_H
#pragma once

#include "ScaleSet.hpp"
#include "ExplainableSet.hpp"
class RandomWordRetriever;

class GuessingDataCreator
{
private:
    ScaleSet::ScaleSetDataRecipe m_scaleSetDataRecipe;
    RandomWordRetriever* m_randomWordRetriever; //This is a pointer to a RandomWordRetriever instance, which is used to acquire word pairs for scales. It is assumed that this pointer is valid and points to an initialized RandomWordRetriever object.
public:
    GuessingDataCreator(ScaleSet::ScaleSetDataRecipe scaleSetDataRecipe, RandomWordRetriever* randomWordRetriever);
    ~GuessingDataCreator();
    std::unique_ptr<ExplainableSet> m_createExplainableSet(); //This uses the explainable set data recipe to create an explainable set.
    std::unique_ptr<ScaleSet> m_createScaleSet(); //This uses the scale set data recipe to create a scale set.
    std::unique_ptr<Scale> m_makeScale(const ScaleSet::ScaleRecipe& recipe); //This uses the scale recipe to create a scale.
    ScaleSet::WordPairForScale m_acquireWordPair(ScaleSet::ScaleType type); //This will acquire a word pair for the given scale type. It will use the scale set data recipe to determine the appropriate word pair.
    void m_setRandomWordRetriever(RandomWordRetriever* randomWordRetriever){m_randomWordRetriever = randomWordRetriever;} //This sets the RandomWordRetriever instance to be used for acquiring word pairs. It is assumed that this pointer is valid and points to an initialized RandomWordRetriever object.

};

#endif