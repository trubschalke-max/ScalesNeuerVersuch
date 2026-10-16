#ifndef ROUND_H
#define ROUND_H
#pragma once

#include "ExplainableSet.hpp"
#include "Scale.hpp"
#include "ObjectData.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include "GameObjectFactory.hpp"

class Round
{
    using Explainable = ObjectData::Explainable;
    using PlayerID = ObjectData::PlayerID;
    using ScaleData = ObjectData::ScaleData;
    using ScaleValue = ObjectData::ScaleValue;
    using WordState = ObjectData::WordState;
    using Recipe = GameObjectFactory::Recipe;
    using GameObjectType = ObjectData::GameObjectType;
public:
    typedef std::map<PlayerID, std::vector<Recipe>> RecipeListing;
    struct RoundData{
        PlayerID explainingPlayer = PlayerID::DUMMY_PLAYER;
        PlayerID guessingPlayer = PlayerID::DUMMY_PLAYER;
        std::unique_ptr<ExplainableSet> explainableSet = nullptr; //Neatly, when these go into history, all the selected explainables and scale values are preserved.
        std::vector<std::unique_ptr<Scale>> scales;
        RoundData(RoundData&&) = default;
        RoundData() = default;
        std::vector<PlayerID> players;
        bool m_areAllScalesCommitted() const;
    };
private:
    RoundData m_roundData;
    unsigned short int m_scalesCount; //Depending on how I will make the rules, this might be wildly oversimplified. It is only relevant until the scales are actually created.
    unsigned short int m_explainablesCount; //Depending on how I will make the rules, this might be wildly oversimplified. It is only relevant until the explainable set is actually created.
    unsigned short int m_scaleMaxValue; //The size of each scale, relevant until the scales are actually created. Note that scale.maxValue - scale.minValue should equal m_scaleMaxValue-1.
    std::vector<std::string> m_generateExplainableWords(); //This shall generate the words for the explainable set, by randomly drawing them from a pool file.
    std::vector<ScaleData> m_generateScaleData(); //This shall generate the data for the scales, by randomly drawing them from pool files. There might be a lot of nuance to the algorithm.
    unsigned short int m_generateCorrectExplainableValue();
    PlayerID m_firstGuessingPlayer() const;
    bool m_shallUpdateGuessingPlayer() const;
    ErrorCode m_updateGuessingPlayer();
    PlayerID m_firstPlayerAfter(PlayerID player, PlayerID excludingPlayer = PlayerID::PLAYER_NONE) const;
    ErrorCode m_createRoundElementRecipes(RecipeListing& recipes);
    ErrorCode m_createRoundElementRecipesForPlayer(PlayerID player, std::vector<Recipe>& recipes);
    ErrorCode m_addScaleElementRecipeForPlayer(PlayerID player, Scale* scale, std::vector<Recipe>& recipes);
    ErrorCode m_addExplainableSetElementRecipeForPlayer(PlayerID player, ExplainableSet* explainableSet, std::vector<Recipe>& recipes);

public:
    Round(PlayerID explainingPlayer, const std::vector<PlayerID>& players, unsigned short int scalesCount, unsigned short int explainablesCount, unsigned short int scaleMaxValue = (int) ScaleValue::SCALE_CANONICAL_END); //The scaleSize parameter 
    ~Round();
    const RoundData& m_RoundData() { return m_roundData; } 
    ErrorCode m_initializeRound(); // This creates the explainable set and scales for the round. It does NOT create the players' game objects.
    ErrorCode m_validate(const Suggestor::Suggestion&); //If the suggestion is valid, returns ErrorCode::OK, if it means nothing, it returns ErrorCode::IGNORE_THIS, otherwise returns an appropriate error code.
    ErrorCode m_handleValidSuggestion(const Suggestor::Suggestion&){std::cout<<"Handling valid suggestion"<<std::endl; return ErrorCode::OK;} //I'll have to commit some time. Thus, a placeholder implementation has to do it. The function assumes validity.
    bool m_isRoundFinished(){return false;} // I guess that this will return true on the correct explainable being selected. Right now, it will always return false.
    friend class Game; //The game needs to std::move RoundData into the round history.

};

#endif