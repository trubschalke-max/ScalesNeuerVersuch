#include "GuessingDataCreator.hpp"
#include "RandomWordRetriever.hpp"

using ScaleType = ScaleSet::ScaleType;

GuessingDataCreator::GuessingDataCreator(ScaleSet::ScaleSetDataRecipe sdr, RandomWordRetriever* r)
    : m_scaleSetDataRecipe(sdr), m_randomWordRetriever(r)
{
    throw std::runtime_error("Not implemented");
}

GuessingDataCreator::~GuessingDataCreator()
{

}

ScaleSet::WordPairForScale GuessingDataCreator::m_acquireWordPair(ScaleSet::ScaleType type){//This assumes that m_randomWordRetriever is properly set and can provide a word pair for the given scale type.
    WordPair wordPair;
    ErrorCode error;
    if(!m_randomWordRetriever)
        throw std::runtime_error("RandomWordRetriever is not set");
    switch(type) {
        case ScaleType::OPPOSITE:
            error = m_randomWordRetriever->m_retrievePair("Opposites", wordPair);
            if(error != ErrorCode::OK)
                throw std::runtime_error("Failed to retrieve opposite word pair");
            return ScaleSet::WordPairForScale{wordPair};
        case ScaleType::RANDOM_PAIR:

            error = m_randomWordRetriever->m_retrieve("General", wordPair.first);
            checkForError(m_randomWordRetriever->m_retrieve("General", wordPair.second), error);
            if(error != ErrorCode::OK)
                throw std::runtime_error("Failed to retrieve random word pair");
            return ScaleSet::WordPairForScale{wordPair};
        default:
            throw std::invalid_argument("Unsupported scale type");
    }
}