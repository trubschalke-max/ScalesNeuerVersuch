#include "ScaleSet.hpp"
#include "RandomWordRetriever.hpp"

ScaleSet::ScaleSet(ScaleSetDataRecipe& sdr, RandomWordRetriever* r)
    : m_randomWordRetriever(r)
{
    for (const auto& scaleRecipe : sdr.scales) {
        scales.push_back(m_makeScale(scaleRecipe));
        rawScales.push_back(scales.back().get());
    }
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

std::unique_ptr<Scale> ScaleSet::m_makeScale(const ScaleRecipe& recipe)
{
    if(recipe.type != ScaleSet::OPPOSITE && recipe.type != ScaleSet::RANDOM_PAIR)
        throw std::invalid_argument("Unsupported scale type");
    ObjectData::ScaleData scaleData;
    scaleData.minValue = recipe.minValue;
    scaleData.maxValue = recipe.maxValue;
    WordPairForScale wordPair = m_acquireWordPair(recipe.type);
    scaleData.above = wordPair.above;
    scaleData.below = wordPair.below;
    return std::make_unique<Scale>(scaleData);
}

ScaleSet::WordPairForScale ScaleSet::m_acquireWordPair(ScaleType type){//This assumes that m_randomWordRetriever is properly set and can provide a word pair for the given scale type.
    WordPair wordPair;
    ErrorCode error;
    if(!m_randomWordRetriever)
        throw std::runtime_error("RandomWordRetriever is not set");
    switch(type) {
        case OPPOSITE:
            error = m_randomWordRetriever->m_retrievePair("Opposites", wordPair);
            if(error != ErrorCode::OK)
                throw std::runtime_error("Failed to retrieve opposite word pair");
            return WordPairForScale{wordPair};
        case RANDOM_PAIR:

            error = m_randomWordRetriever->m_retrieve("General", wordPair.first);
            checkForError(m_randomWordRetriever->m_retrieve("General", wordPair.second), error);
            if(error != ErrorCode::OK)
                throw std::runtime_error("Failed to retrieve random word pair");
            return WordPairForScale{wordPair};
        default:
            throw std::invalid_argument("Unsupported scale type");
    }
}