#ifndef SCALESET_H
#define SCALESET_H
#pragma once

#include <map>
#include "Scale.hpp"
#include <memory>

class RandomWordRetriever;

struct WordPair;

class ScaleSet //From now on, this class will manage a collection of Scale objects based on the provided ScaleSetDataRecipe. The round will hold one ScaleSet.
{
public:
    enum ScaleType //Maybe, I have to extend this by further information, like mutability or topics.
    {
        OPPOSITE, RANDOM_PAIR
    };
    struct ScaleRecipe{
        ScaleType type;
        ObjectData::ScaleValue maxValue;
        ObjectData::ScaleValue minValue;
        //Mutability data?
    };
    struct ScaleSetDataRecipe{
        //Consider above-below string pairs for scales. They may be lexical opposites of each other, or not.
        std::vector<ScaleRecipe> scales;
        ScaleSetDataRecipe() = default; // Default constructor
        ScaleSetDataRecipe(std::map<ScaleType, int>, ObjectData::ScaleValue maxValue = ObjectData::ScaleValue::SCALE_CANONICAL_END, ObjectData::ScaleValue minValue = ObjectData::ScaleValue::SCALE_CANONICAL_START);
    };
    struct WordPairForScale
    {
        std::string above;
        std::string below;
        WordPairForScale(const WordPair& wp);
    };
private:
    std::vector<std::unique_ptr<Scale>> m_scales;
    std::vector<Scale*> m_rawScales;

public:
    ScaleSet();
    ~ScaleSet();
    const std::vector<Scale*>& m_getRawScales() const { return m_rawScales; }
};

#endif