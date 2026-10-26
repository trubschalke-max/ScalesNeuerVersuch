#ifndef EXPLAINABLESET_H
#define EXPLAINABLESET_H
#pragma once

#include <string>
#include <vector>
#include "ObjectData.hpp"
#include "myExceptions.hpp"

class ExplainableSet{
private:
    std::vector<ObjectData::Explainable> m_explainables;
    std::vector<ObjectData::ExplainableSetValue> m_correctValues(); 

public:
    ExplainableSet(const std::vector<std::string>& words, ObjectData::ExplainableSetValue selectedValue = ObjectData::ExplainableSetValue::EXSET_NONE);
    virtual ~ExplainableSet();

    ObjectData::ExplainableSetValue m_correctValue(); //This function returns the first correct value, or EXSET_NONE if there is none. If there are multiple correct values, it will throw a MultipleCorrectWords exception. This function is intended for use in cases where there is only one correct value, and the user is expected to select it.
    const std::vector<ObjectData::Explainable>& m_Explainables(){return m_explainables;}
    ErrorCode m_setCorrectValue(ObjectData::ExplainableSetValue value); //This function sets the correct value. If the value is not in the set, it returns INVALID_VALUE. If there are multiple correct values, it returns MULTIPLE_CORRECT_VALUES. If the value is already correct, it returns ALREADY_CORRECT. Otherwise, it sets the value to be correct and returns OK.
    ErrorCode m_guess(ObjectData::ExplainableSetValue value); //This function marks the guessed value as tried. If the value is not in the set, it returns INVALID_WORD_SELECTION. If the value has already been tried, it returns WORD_ALREADY_TRIED. If the value is wrong, it returns ErrorCode::SUCCESS. If the value is correct, it returns ErrorCode::CORRECT_WORD_WAS_JUST_SELECTED.^
    bool m_contains(ObjectData::ExplainableSetValue value);
    bool m_isCorrectlyGuessed(); //This function returns true if the correct value has been guessed, false otherwise.
    int m_numberOfGuesses(); //This function returns the number of guesses made so far.
};

struct MultipleCorrectWords: public std::exception{
    const char* what() const noexcept override;
    std::vector<ObjectData::ExplainableSetValue> m_correctWords;
};

#endif