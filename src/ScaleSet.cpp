#include "ScaleSet.hpp"
#include "RandomWordRetriever.hpp"

ScaleSet::ScaleSet()
{
}

ScaleSet::WordPairForScale::WordPairForScale(const WordPair& wp)
    : above(wp.first), below(wp.second)
{
}

ScaleSet::~ScaleSet()
{

}

ScaleSet::ScaleSetDataRecipe::ScaleSetDataRecipe(std::map<ScaleType, int> scaleCounts, ObjectData::ScaleValue maxValue, ObjectData::ScaleValue minValue)
{
    for (const auto& pair : scaleCounts) {
        auto type = pair.first;
        auto count = pair.second;
        for (int i = 0; i < count; ++i) {
            scales.push_back(ScaleRecipe{type, maxValue, minValue});
        }
    }
}
