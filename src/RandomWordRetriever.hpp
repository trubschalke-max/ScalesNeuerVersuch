#ifndef RANDOMWORDRETRIEVER_H
#define RANDOMWORDRETRIEVER_H
#pragma once

#include <fstream>
#include "ErrorCode.hpp"
#include <vector>
#include <map>
#include <random>
#include <stdexcept>

struct WordPair{//This struct represents a pair of words, typically used to store two opposite words together.
    std::string first;
    std::string second;
    bool operator==(const WordPair& other) const{
        return first == other.first && second == other.second;
    }
    int m_similarityScore(const WordPair& other) const{//Returns a similarity score based on matching first and second words. First word match adds 1, second word match adds 2. (amounting to a bit flag representation)
        int score = 0;
        if(first == other.first) score+=1;
        if(second == other.second) score+=2;
        return score;
    }
};

class RandomWordRetriever //This class can be used to retrieve random words from a predefined set, which is stored in a json file. The filename will be passed when the m_retrieve() function is called. In the future, I may use an interface to allow different sources for (random or not random) words, particularly from a different peer.
{
public: 
    enum class Mode{ZERO = 0, CARELESS = 0, NO_EMPTY_VECTORS = 1, NO_DUPLICATES_WITH_PRIOR_ELEMENTS = 2, NO_REPEATED_RETRIEVALS = 4, OVERWRITE_ON_DUPLICATE_LOAD = 8, EXTEND_ON_DUPLICATE_LOAD = 16, SKIP_ON_DUPLICATE_LOAD = 32}; //Setting more of one out of OVERWRITE_ON_DUPLICATE_LOAD, SKIP_ON_DUPLICATE_LOAD and EXTEND_ON_DUPLICATE_LOAD simultaneously may lead to undefined behavior.

private:
    std::map<std::string, std::vector<std::string>> m_wordPools;
    std::map<std::string, std::vector<WordPair>> m_wordPairPools;
    std::map<std::string, std::vector<std::string>> m_wordsAlreadyRetrieved;
    std::map<std::string, std::vector<WordPair>> m_wordPairsAlreadyRetrieved;
    Mode m_mode = static_cast<Mode>((int) Mode::NO_EMPTY_VECTORS | (int) Mode::NO_DUPLICATES_WITH_PRIOR_ELEMENTS | (int) Mode::NO_REPEATED_RETRIEVALS | (int) Mode::OVERWRITE_ON_DUPLICATE_LOAD);
    std::random_device m_randomDevice;
    std::ifstream m_file;
    std::mt19937 m_randomEngine{m_randomDevice()};
    ErrorCode m_loadInternally(const std::string& wordPoolName, bool avoidDuplicates = true); //without the check for duplicate loads or file opening. If avoidDuplicates is true, every word will only be added if it is not already present in the pool. It is assumed that the file is already open and ready to be read, and the word (pair) pool exists both in the JSON file and in the RandomWordRetriever instance.
    ErrorCode m_retrieveInternally(int howMany, const std::string& wordPoolName, std::vector<std::string>& words); //without the check for empty output vectors
    ErrorCode m_retrieveInternally(const std::string& wordPoolName, std::string& word); //without the check for empty output vectors or non-empty output strings or if the word pool exists
    ErrorCode m_retrievePairInternally(const std::string& wordPoolName, WordPair& wordPair); //without the check for empty output vectors or non-empty output strings or if the word pool exists
public:
    RandomWordRetriever();
    ~RandomWordRetriever();
    void m_setPermitNonEmptyOutputVectors(Mode mode){ m_mode = mode; }
    ErrorCode m_open(const std::string& filename);
    ErrorCode m_loadWordPool(const std::string& wordPoolName);
    ErrorCode m_loadWordPools(const std::vector<std::string>& wordPoolNames);
    ErrorCode m_retrieve(const std::string& wordPoolName, std::string& word);
    ErrorCode m_retrieve(int howMany,  const std::string& wordPoolName, std::vector<std::string>& words);//This function does the same as m_retrieve(const std::string&, std::string&) but retrieves multiple words at once, with additional checks for the validity of the 'howMany' argument and non-empty output vectors if those are not permitted.  
    ErrorCode m_retrieve(const std::map<std::string, int>& howManyPerPool, std::vector<std::string>& words);//Note that this function will not protect against duplicates where the words have come from different pools. I'm not sure how I would implement such protection efficiently. If an odd number of words is requested from a pool of word pairs, INVALID_ODD_NUMBER_OF_WORD_REQUESTS will be returned.
    ErrorCode m_retrievePair(const std::string& wordPoolName, WordPair& wordPair); 
    ErrorCode m_close();

};

RandomWordRetriever::Mode operator|(RandomWordRetriever::Mode lhs, RandomWordRetriever::Mode rhs);
RandomWordRetriever::Mode operator&(RandomWordRetriever::Mode lhs, RandomWordRetriever::Mode rhs);
bool containsFlags(RandomWordRetriever::Mode lhs, RandomWordRetriever::Mode rhs);

class myLoaderException : public std::runtime_error {
public:
    explicit myLoaderException(const std::string& message) : std::runtime_error(message) {}
};

#endif