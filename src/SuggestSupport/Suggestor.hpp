#ifndef SUGGESTOR_H
#define SUGGESTOR_H

#pragma once
#include "ObjectData.hpp"
#include <memory>
#include "ErrorCode.hpp"
#include <vector>

class GameObject;
class ButtonElement;

class Suggestor
{
public:
    struct Suggestion{//Note: Any suggestion should be safe to be applied by multiple game objects. If one solitary change is meant to be made, subsequent putting into effect shall be able to return NOTHING_HAPPENED and not do any damage.
        enum ApplicabilityType{ONCE = 1, MULTIPLE = -1, ERROR = -2};
        enum Radicality{NOTHING_AT_ALL, SELECTION_ONLY, FULL_CHANGE};
        ApplicabilityType applicability = ApplicabilityType::ONCE;
        ObjectData::PlayerID playerID = ObjectData::PlayerID::PLAYER_NONE;
        std::unique_ptr<ObjectData::Activeness> activeness; //This shall replace std::optionals. Unfortunately, my cpp standard is not as high.
        ButtonElement* buttonElement = nullptr;
        std::unique_ptr<ObjectData::ScaleData> scaleData;
        std::unique_ptr<std::vector<ObjectData::Explainable>> explainables;
        std::unique_ptr<ObjectData::ExplainableSetValue> selectedWordValue;
        bool m_empty() const;
        static Suggestion None(){ return Suggestion{};}
        ErrorCode certainlyKnownError = ErrorCode::OK; 
        ErrorCode m_validate() const; //Validates the suggestion. Returns an appropriate error code if the suggestion is invalid. It returns certainlyKnownError if that is neither set nor part of ErrorHandling::standardErrorHierarchy().
        Radicality radicality = Radicality::NOTHING_AT_ALL; //Admittedly, this circumvents using a scale data field and a scale selection field.
        GameObject* target = nullptr; //This can be used together with full radicality to indicate the specific game object affected by the suggestion.
    };
private: 
    GameObject* const m_associatedObject;
protected:
    GameObject* const m_AssociatedObject(){return m_associatedObject;}
    virtual Suggestion m_suggestForRelevantEvent(const SDL_Event& event); //The default behavior will be to return an empty suggestion.
    Suggestor::Suggestion::Radicality m_standardRadicality = Suggestor::Suggestion::Radicality::SELECTION_ONLY;
    virtual Suggestion::Radicality m_determineRadicality(const SDL_Event& event); //The default behavior will be only to return m_standardRadicality.

public:
    Suggestor(GameObject* obj);
    virtual ~Suggestor();
    Suggestion m_suggestFor(const SDL_Event& event); //This checks for relevance and redirects to m_suggestForRelevantEvent().
    virtual bool m_isRelevantForSuggesting(const SDL_Event& event); //The default behavior will be to always return false.
};

#endif