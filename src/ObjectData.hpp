#ifndef OBJECT_DATA_HPP
#define OBJECT_DATA_HPP
#pragma once

#include <String>
#include <SDL2/SDL.h>

namespace ObjectData{
 enum class State{COMMITTED, NOT_YET_COMMITTED, ERROR}; 
    enum class ScaleValue{SCALE_COMMIT = -1, SCALE_UNDO = -2, SCALE_NO_VALUE = -3, SCALE_0 = 0, SCALE_1, SCALE_2, SCALE_3, SCALE_4, SCALE_CANONICAL_END = 4,
    NOT_CLICKED = -4, CLICKING_CONFLICT = -5, DUMMY_VALUE = -6
    }; //One tile below the scale, there's an undo button. Two tiles below, there's a commit button.
    struct ScaleData{
        std::string above;
        std::string below;
        ScaleValue selection;
        ScaleValue minValue = ScaleValue::SCALE_0;
        ScaleValue maxValue = ScaleValue::SCALE_CANONICAL_END;
        State state = State::NOT_YET_COMMITTED;
        bool coincidesWith(const ScaleData& other){
            return (above == other.above) && (below == other.below) && (selection == other.selection) && (minValue == other.minValue) && (maxValue == other.maxValue) && (state == other.state);
        }
    };
    enum class Activeness{
        ACTIVE = 1, HOVERED_OVER = 2, HOVERED_OVER_AND_ACTIVE = HOVERED_OVER | ACTIVE, PASSIVE = 0, HOVERED_OVER_AND_PASSIVE = HOVERED_OVER | PASSIVE, FORBIDDEN = 4, PERMANENTLY_SELECTED = ACTIVE | FORBIDDEN, ACTIVENESS_ERROR = 8
    };
    enum class WordState{WRONG = 0, CORRECT = 1, TRIED = 2, WRONG_AND_TRIED = WRONG | TRIED, CORRECTLY_TRIED = CORRECT | TRIED};
    bool m_meansHoveredOver(Activeness ac);
    enum ExplainableSetValue{EXSET_0 = 0, EXSET_NONE = -30, EXSET_1 = 1, EXSET_2 = 2, EXSET_3 = 3, EXSET_4=4,EXSET_5=5, EXSET_6=6, EXSET_7=7, EXSET_8=8, EXSET_9=9, EXSET_10=10, EXSET_11=11, EXSET_12=12, EXSET_13=13, EXSET_14=14, EXSET_15=15, EXSET_16=16, EXSET_17=17, EXSET_18=18, EXSET_19=19, EXSET_20 = 20, EXSET_21 = 21, EXSET_22 = 22, EXSET_23 = 23, EXSET_24 = 24, EXSET_25 = 25, EXSET_ERROR = -200};
    struct Explainable{
        std::string word;
        ObjectData::WordState state;
    };
}
std::string toString(ObjectData::Activeness ac);
std::string toString(ObjectData::WordState ws);
ObjectData::Activeness operator|(ObjectData::Activeness a, ObjectData::Activeness b);
ObjectData::WordState operator|(ObjectData::WordState a, ObjectData::WordState b);
ObjectData::Activeness operator&(ObjectData::Activeness a, ObjectData::Activeness b);
ObjectData::WordState operator&(ObjectData::WordState a, ObjectData::WordState b);


#endif