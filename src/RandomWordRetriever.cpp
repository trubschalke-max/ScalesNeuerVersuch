#include "RandomWordRetriever.hpp"
#include <random>
#include <algorithm>
#include "includeJson/single_include/nlohmann/json.hpp" 
#include <iostream>
#include <stdexcept>

using Json = nlohmann::json;

RandomWordRetriever::Mode operator|(RandomWordRetriever::Mode lhs, RandomWordRetriever::Mode rhs){
    return static_cast<RandomWordRetriever::Mode>(static_cast<int>(lhs) | static_cast<int>(rhs));
}
RandomWordRetriever::Mode operator&(RandomWordRetriever::Mode lhs, RandomWordRetriever::Mode rhs){
    return static_cast<RandomWordRetriever::Mode>(static_cast<int>(lhs) & static_cast<int>(rhs));
}
bool containsFlags(RandomWordRetriever::Mode lhs, RandomWordRetriever::Mode rhs){
    return (lhs & rhs) != RandomWordRetriever::Mode::ZERO;
}

RandomWordRetriever::RandomWordRetriever()
{

}

RandomWordRetriever::~RandomWordRetriever()
{

}

ErrorCode RandomWordRetriever::m_retrieve(const std::string& wordPoolName, std::string& word){
    if(!word.empty()){
        return ErrorCode::NONEMPTY_WORD;
    }
    if(m_wordPairPools.find(wordPoolName) != m_wordPairPools.end()){
        return ErrorCode::WORD_PAIR_POOL_NAME_FOR_WORD_POOL;
    }
    auto it = m_wordPools.find(wordPoolName);
    if(it == m_wordPools.end()){
        return ErrorCode::WORD_POOL_NOT_FOUND;
    }
    auto& words = it->second;
    if(words.empty()){
        return ErrorCode::WORD_POOL_EMPTY;
    }
    ErrorCode error = m_retrieveInternally(wordPoolName, word);
    //I could check for more things here
    return error;
}

ErrorCode RandomWordRetriever::m_retrieve(int howMany, const std::string& wordPoolName, std::vector<std::string>& words){
    if(howMany <= 0){
        return ErrorCode::INVALID_HOWMANY_ARGUMENT;
    }
    if((m_mode & Mode::NO_EMPTY_VECTORS) != Mode::ZERO && !words.empty()){
        return ErrorCode::NONEMPTY_WORD;
    }
    return m_retrieveInternally(howMany, wordPoolName, words);
}

ErrorCode RandomWordRetriever::m_retrieve(const std::map<std::string, int>& howManyPerPool, std::vector<std::string>& words){
    for(const auto pair : howManyPerPool){
        auto poolName = pair.first;
        auto howMany = pair.second;
        ErrorCode ec = m_retrieve(howMany, poolName, words);
        if(ec != ErrorCode::OK){
            return ec;
        }
    }
    return ErrorCode::OK;
}

ErrorCode RandomWordRetriever::m_retrievePair(const std::string& wordPoolName, WordPair& wordPair){
    if((m_mode & Mode::NO_EMPTY_VECTORS) != Mode::ZERO){
        if(!wordPair.first.empty() || !wordPair.second.empty()){
            return ErrorCode::NONEMPTY_WORD;
        }
    }
    //Check if it is actually a pool of single words
    if(m_wordPools.find(wordPoolName) != m_wordPools.end()){
        return ErrorCode::WORD_POOL_NAME_FOR_WORD_PAIR;
    }
    auto it = m_wordPairPools.find(wordPoolName);
    if(it == m_wordPairPools.end()){
        return ErrorCode::WORD_POOL_NOT_FOUND;
    }
    auto& wordPairs = it->second;
    if(wordPairs.empty()){
        return ErrorCode::WORD_POOL_EMPTY;
    }
    ErrorCode error = m_retrievePairInternally(wordPoolName, wordPair);
    //I could check for more things here
    return error;
}

ErrorCode RandomWordRetriever::m_open(const std::string& filename){
    if(m_file.is_open()){
        return ErrorCode::FILE_ALREADY_OPEN;
    }
    m_file.open(filename);
    if(!m_file.is_open()){
        return ErrorCode::FILE_NOT_FOUND;
    }
    return ErrorCode::OK;
}

ErrorCode RandomWordRetriever::m_close(){
    if(!m_file.is_open()){
        return ErrorCode::FILE_NOT_OPEN;
    }
    m_file.close();
    return ErrorCode::OK;
}

ErrorCode RandomWordRetriever::m_retrieveInternally(int howMany, const std::string& wordPoolName, std::vector<std::string>& words){
    ErrorCode error = ErrorCode::OK;
    if(m_wordPools.find(wordPoolName) != m_wordPools.end()){
        m_wordPools[wordPoolName] = std::vector<std::string>();
    }
    if(m_wordsAlreadyRetrieved.find(wordPoolName) == m_wordsAlreadyRetrieved.end() && (m_mode & Mode::NO_REPEATED_RETRIEVALS) != Mode::ZERO){
        m_wordsAlreadyRetrieved[wordPoolName] = std::vector<std::string>();
    }
    if(words.size() < howMany){
        return ErrorCode::INVALID_HOWMANY_ARGUMENT;
    }
    auto& wordPool = m_wordPools[wordPoolName];
    auto& wordsAlreadyRetrieved = m_wordsAlreadyRetrieved[wordPoolName];
    if(howMany > wordPool.size()){
        return ErrorCode::TOO_SMALL_WORD_POOL;
    }
    for(int i = 0; i < howMany; ++i){
        std::string word;
        checkForError(m_retrieveInternally(wordPoolName, word), error);
        words[i] = word;
    }
    return ErrorCode::OK;
}

ErrorCode RandomWordRetriever::m_retrievePairInternally(const std::string& wordPoolName, WordPair& wordPair){
    if(m_wordPairPools.find( wordPoolName) != m_wordPairPools.end()){
        m_wordPairPools[wordPoolName] = std::vector<WordPair>();
    }
    if(m_wordPairsAlreadyRetrieved.find(wordPoolName) == m_wordPairsAlreadyRetrieved.end() && (m_mode & Mode::NO_REPEATED_RETRIEVALS) != Mode::ZERO){
        m_wordPairsAlreadyRetrieved[wordPoolName] = std::vector<WordPair>();
    }
    auto& wordPairs = m_wordPairPools[wordPoolName];
    auto& wordPairsAlreadyRetrieved = m_wordPairsAlreadyRetrieved[wordPoolName]; 
    std::uniform_int_distribution<> dis(0, wordPairs.size() - 1);
    int index = dis(m_randomEngine);
    wordPair = wordPairs[index];
    if(containsFlags(m_mode, Mode::NO_REPEATED_RETRIEVALS)){
        if(std::find(wordPairsAlreadyRetrieved.begin(), wordPairsAlreadyRetrieved.end(), wordPair) != wordPairsAlreadyRetrieved.end()){
            return ErrorCode::REPEATED_RETRIEVAL;
        }
        wordPairsAlreadyRetrieved.push_back(wordPair);
    }
    return ErrorCode::OK;
}

ErrorCode RandomWordRetriever::m_loadWordPools(const std::vector<std::string>& wordPoolNames){
    ErrorCode error = ErrorCode::OK;
    for(const auto& wordPoolName : wordPoolNames){
        checkForError(m_loadWordPool(wordPoolName), error);
    }
    return error;
}
ErrorCode RandomWordRetriever::m_loadWordPool(const std::string& wordPoolName){
ErrorCode error = ErrorCode::OK;
try{
    if(m_wordPools.find(wordPoolName) == m_wordPools.end() || containsFlags(m_mode, Mode::OVERWRITE_ON_DUPLICATE_LOAD)){//If the pool is meant to be overwritten, it means that the pool is being loaded for the first time or should be replaced due to the overwrite flag.
        m_wordPools[wordPoolName] = std::vector<std::string>();
    }
    if(!containsFlags(m_mode, Mode::EXTEND_ON_DUPLICATE_LOAD) && !containsFlags(m_mode, Mode::SKIP_ON_DUPLICATE_LOAD) && m_wordPools.find(wordPoolName) != m_wordPools.end()){
        error = ErrorCode::DUPLICATE_LOAD_NOT_ALLOWED;
    }
    error = m_loadInternally(wordPoolName);
} catch(const std::ios::failure& e){
    std::cerr << "File error: " << e.what() << std::endl;
    error = ErrorCode::FILE_NOT_OPEN;
}
    catch(nlohmann::json::exception& e){
        std::cerr << "JSON error: " << e.what() << std::endl;
        error = ErrorCode::GENERAL_LOADING_ERROR;
    }
return error;
}

ErrorCode RandomWordRetriever::m_loadInternally(const std::string& wordPoolName, bool avoidDuplicates){
try{
    // The json file will have a subjson "wordpools", which contains objects (accessed via wordPoolName) corresponding to the word pool, which contains the words for that pool (named "words"), and other metadata. Particularly, there is an attribute "name", and an attributes "wordsPerUnit", of which this class only accepts single words or pairs. If "wordsPerUnit" is 1, a unit will not be an array, but a single string instead.
    if(!m_file.is_open()){
        return ErrorCode::FILE_NOT_OPEN;
    }
    Json jsonData;
    m_file.seekg(0); //Set the file pointer to the beginning of the file
    m_file >> jsonData;
    if(jsonData.find("wordPools") == jsonData.end()){
        return ErrorCode::NO_WORD_POOLS_IN_FILE;
    }
    jsonData = jsonData["wordPools"]; //forget about the outer json
    if(jsonData.find(wordPoolName) == jsonData.end()){
        return ErrorCode::WORD_POOL_NOT_FOUND;
    }
    auto& wordPoolJson = jsonData[wordPoolName];
    if(wordPoolJson.find("words") == wordPoolJson.end()){
        return ErrorCode::EMPTY_WORD_POOL_IN_FILE;
    }
    auto& words = wordPoolJson["words"];
    if(words.find("wordsPerUnit") == words.end()){
        throw myLoaderException("Missing wordsPerUnit attribute");
    }
    auto& wordsPerUnit = words["wordsPerUnit"];
    if(wordsPerUnit != 1 && wordsPerUnit != 2){// Only single words or pairs are accepted. Note that this is a legal overload.
        throw myLoaderException("Invalid wordsPerUnit value: " + std::to_string(wordsPerUnit.get<int>()));
    }
    if(!words.is_array()){
        return ErrorCode::INVALID_WORDS_FORMAT;
    }
    if(wordsPerUnit == 1){
        for(auto& unit : words){
            auto& singleWordPool = m_wordPools[wordPoolName]; 
            if(!unit.is_string()){
                throw myLoaderException("Invalid single word format: not a string");
            }
            if(avoidDuplicates){
                if(std::find(singleWordPool.begin(), singleWordPool.end(), unit.get<std::string>()) != singleWordPool.end()){
                    continue; // Skip duplicate word
                }
            }
            else{
                singleWordPool.push_back(unit.get<std::string>());
            }
        }
    }
    if(wordsPerUnit == 2){
        for(auto& unit : words){
            auto& wordPairPool = m_wordPairPools[wordPoolName];
            if(!unit.is_array() || unit.size() != 2){
                throw myLoaderException("Invalid word pair format: not a suitable array");
            }
            if(!unit[0].is_string() || !unit[1].is_string()){
                throw myLoaderException("Invalid word pair format: not only strings in array");
            }
            WordPair pair{unit[0].get<std::string>(), unit[1].get<std::string>()};
            if(avoidDuplicates){
                if(std::find(wordPairPool.begin(), wordPairPool.end(), pair) != wordPairPool.end()){
                    continue; // Skip duplicate word pair
                }
            }
            wordPairPool.push_back(pair);
        }
    }
    return ErrorCode::OK;
}
catch(const std::ios::failure& e){
    std::cerr << "File error: " << e.what() << std::endl;
    return ErrorCode::FILE_NOT_OPEN;
}
catch(nlohmann::json::exception& e){
    std::cerr << "JSON error: " << e.what() << std::endl;
    return ErrorCode::GENERAL_LOADING_ERROR;
}
catch(myLoaderException& e){
    std::cerr << "Loader error: " << e.what() << std::endl;
    return ErrorCode::GENERAL_LOADING_ERROR;
}
catch(...){
    throw;
}
}

ErrorCode RandomWordRetriever::m_retrieveInternally(const std::string& wordPoolName, std::string& word) {
    auto& wordPool = m_wordPools[wordPoolName];
    auto& wordsAlreadyRetrieved = m_wordsAlreadyRetrieved[wordPoolName];
    std::uniform_int_distribution<> dis(0, wordPool.size() - 1);
    int index = dis(m_randomEngine);
    std::string randomWord = wordPool[index];
    if((m_mode & Mode::NO_REPEATED_RETRIEVALS) != Mode::ZERO){
        wordPool.erase(wordPool.begin() + index);
        if(std::find(wordsAlreadyRetrieved.begin(), wordsAlreadyRetrieved.end(), randomWord) != wordsAlreadyRetrieved.end()){
            return ErrorCode::REPEATED_RETRIEVAL; // This indicates that the word has already been retrieved previously and repeated retrieval is not allowed. I can hardly imagine this happening without a major bug being present.
        }
        wordsAlreadyRetrieved.push_back(randomWord);
    }
    word = randomWord;
    return ErrorCode::OK;
}