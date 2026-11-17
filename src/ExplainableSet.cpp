#include "ExplainableSet.hpp"
#include <sstream>
#include <iostream>

using ObjectData::WordState;
using ObjectData::ExplainableSetValue;
using ObjectData::Explainable;

ExplainableSet::ExplainableSet(const std::vector<std::string>& words, ExplainableSetValue selectedValue)
{
    std::vector<Explainable> tmpExplainables;
    m_explainables.reserve(words.size());
    for(int i(0); i < words.size(); ++i){
        auto& word = words[i];
        Explainable e{word, (int) selectedValue == i ? WordState::CORRECT : WordState::WRONG};
        m_explainables.push_back(e);
    }
}

ExplainableSet::~ExplainableSet()
{

}

std::vector<ExplainableSetValue> ExplainableSet::m_correctValues(){
    std::vector<ExplainableSetValue> correctWords;
    for(int i(0); i<m_explainables.size();++i){
        if(m_explainables[i].state == WordState::CORRECT){
            correctWords.push_back((ExplainableSetValue) i);
        }
    }
    return correctWords;
}

ExplainableSetValue ExplainableSet::m_correctValue(){
    auto correctWords = m_correctValues();
    if(correctWords.size() == 0){
        return ExplainableSetValue::EXSET_NONE;
    }
    else if(correctWords.size() == 1){
        return correctWords[0];
    }
    else{
        MultipleCorrectWords mcw;
        mcw.m_correctWords = correctWords;
        throw mcw;
    }
}

char const* MultipleCorrectWords::what() const noexcept{
    std::stringstream s; s << "Multiple correct words found in ExplainableSet. Namely: ";
    for(int i(0); i < m_correctWords.size(); ++i){
        s << (int) m_correctWords[i] << " ";
    }
    s.flush();
    return s.str().c_str();
}

ErrorCode ExplainableSet::m_setCorrectValue(ExplainableSetValue value){
    if((int) value < 0 || (int) value >= m_explainables.size()){
        return ErrorCode::INVALID_WORD_SELECTION;
    }
    auto correctWords = m_correctValues();
    if(correctWords.size() > 1){
        return ErrorCode::MULTIPLE_CORRECT_VALUES;
    }
    else if(correctWords.size() == 1 && correctWords[0] == value){
        return ErrorCode::CORRECT_WORD_ALREADY_SELECTED;
    }
    else{
        for(int i(0); i < m_explainables.size(); ++i){
            m_explainables[i].state = (i == (int) value) ? WordState::CORRECT : WordState::WRONG;
        }
        return ErrorCode::OK;
    }
}

ErrorCode ExplainableSet::m_guess(ExplainableSetValue value){
    if((int) value < 0 || (int) value >= m_explainables.size()){
        return ErrorCode::INVALID_WORD_SELECTION;
    }
    Explainable& explainable = m_explainables[(int) value];
    if((explainable.state & WordState::TRIED) == WordState::TRIED){//Maybe it would make more sense to check for the few concrete values that are valid.
        if(explainable.state == WordState::CORRECTLY_TRIED){
            return ErrorCode::CORRECT_WORD_ALREADY_SELECTED;
        }
        return ErrorCode::WORD_ALREADY_TRIED;
    }
    else{
        explainable.state = (WordState)((int) explainable.state | (int) WordState::TRIED);
        if(explainable.state == WordState::CORRECTLY_TRIED){
            return ErrorCode::CORRECT_WORD_WAS_JUST_SELECTED;
        }
        return ErrorCode::SUCCESS;
    }
}

bool ExplainableSet::m_contains(ExplainableSetValue value){
    if((int) value < 0 || (int) value >= m_explainables.size()){
        return false;
    }
    return true;
}

bool ExplainableSet::m_isCorrectlyGuessed(){
    for(int i(0); i < m_explainables.size(); ++i){
        if(m_explainables[i].state == WordState::CORRECTLY_TRIED){
            return true;
        }
    }
    return false;
}

int ExplainableSet::m_numberOfGuesses(){
    int count = 0;
    for(int i(0); i < m_explainables.size(); ++i){
        if((m_explainables[i].state & WordState::TRIED) == WordState::TRIED){
            ++count;
        }
    }
    return count;
}