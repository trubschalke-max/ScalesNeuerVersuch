#ifndef EXPLAINABLESET_H
#define EXPLAINABLESET_H
#pragma once

#include <string>
#include <vector>
#include "ObjectData.hpp"

class ExplainableSet
{
    enum ExplainableSetValue{EXSET_0 = 0, EXSET_NONE = -30, EXSET_1 = 1, EXSET_2 = 2, EXSET_3 = 3, EXSET_4=4,EXSET_5=5, EXSET_6=6, EXSET_7=7, EXSET_8=8, EXSET_9=9, EXSET_10=10, EXSET_11=11, EXSET_12=12, EXSET_13=13, EXSET_14=14, EXSET_15=15, EXSET_16=16, EXSET_17=17, EXSET_18=18, EXSET_19=19, EXSET_20 = 20, EXSET_21 = 21, EXSET_22 = 22, EXSET_23 = 23, EXSET_24 = 24, EXSET_25 = 25, EXSET_ERROR = -200};
    struct Explainable{
        bool m_isCorrect;
        std::string word;

        ObjectData::WordState state;
    };
private:
    std::vector<Explainable> m_explainables;
    bool m_isValid(std::vector<Explainable>& explainables) const;
    bool m_isValid() const;

public:
    ExplainableSet(std::vector<std::string> words, ExplainableSetValue selectedValue = EXSET_NONE);
    virtual ~ExplainableSet();


};

#endif