#ifndef SUGGESTOR_H
#define SUGGESTOR_H

#pragma once
#include "ObjectData.hpp"
#include <memory>
#include "ErrorCode.hpp"
#include <vector>

class GameObject;

class Suggestor
{
public:
    struct Suggestion{
        std::unique_ptr<ObjectData::Activeness> activeness; //This shall replace std::optionals. Unfortunately, my cpp standard is not as high.
        std::unique_ptr<ObjectData::ScaleData> scaleData;
        std::unique_ptr<std::vector<ObjectData::Explainable>> explainables;
        std::unique_ptr<ObjectData::ExplainableSetValue> selectedWordValue;
        bool m_empty() const;
        static Suggestion None(){ return Suggestion{};}
    };
private: 
    GameObject* const m_associatedObject;
protected:
    GameObject* const m_AssociatedObject(){return m_associatedObject;}
    virtual Suggestion m_suggestForRelevantEvent(const SDL_Event& event); //The default behavior will be to return an empty suggestion.
public:
    Suggestor(GameObject* obj);
    virtual ~Suggestor();
    Suggestion m_suggestFor(const SDL_Event& event); //This checks for relevance and redirects to m_suggestForRelevantEvent().
    virtual bool m_isRelevantForSuggesting(const SDL_Event& event); //The default behavior will be to always return false.
};

#endif