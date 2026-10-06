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
    bool m_meansHoveredOver(Activeness ac);
}


#endif