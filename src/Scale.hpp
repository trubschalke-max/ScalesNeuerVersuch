#ifndef SCALE_HPP
#define SCALE_HPP
#include<string>
#include"ErrorCode.hpp"
#include<map>
#include<SDL2/SDL.h>
#include "GameObject.hpp"

class Scale{
public: 
    enum State{COMMITTED, NOT_YET_COMMITTED, ERROR}; 
    enum ScaleValue{SCALE_COMMIT = -1, SCALE_UNDO = -2, SCALE_NO_VALUE = -3, SCALE_0 = 0, SCALE_1, SCALE_2, SCALE_3, SCALE_4, SCALE_CANONICAL_END = 4,
    NOT_CLICKED = -4, CLICKING_CONFLICT = -5, DUMMY_VALUE = -6
    }; //One tile below the scale, there's an undo button. Two tiles below, there's a commit button.
    struct ScaleData{
        std::string above;
        std::string below;
        ScaleValue selection;
        ScaleValue minValue = SCALE_0;
        ScaleValue maxValue = SCALE_CANONICAL_END;
        State state = State::NOT_YET_COMMITTED;
    };
private:
    std::map<SDL_Renderer*, std::map<ScaleValue, SDL_Rect>> m_currentButtons;
    ScaleData m_data;

    
    public:
    Scale(std::string above, std::string below, ScaleValue selection = SCALE_NO_VALUE);
    Scale(const ScaleData& data);
    virtual ~Scale();
    const ScaleData& m_Data(){return m_data;}
    
    ErrorCode m_select(ScaleValue newValue); 
    ErrorCode m_commit(); //This will make the current selection final. Most notably, the state will be set to COMMITTED. In case it was committed already, it returns ALREADY_COMMITTED. In case nothing was selected, it returns INVALID_COMMIT. If state was ERROR  before, FAILURE is returned.
    ErrorCode m_undo(); //This will undo the previous selection.In case it was committed already, it returns ALREADY_COMMITTED. In case nothing was selected, it returns INVALID_COMMIT. If state was ERROR  before, FAILURE is returned.
    std::string m_textAt(ScaleValue nr);
    ScaleValue m_scaleValueForCommitting(){return (ScaleValue) ((int) m_data.minValue - 1);}
    ScaleValue m_scaleValueForUndoing(){return (ScaleValue) ((int) m_data.minValue - 2);}
    bool m_contains(ScaleValue sv); //Returns true on any value between the minimum and maximum of the regular scale values (excluding further buttons)
};

typedef Scale::ScaleValue Svalue;


#endif // SCALE_HPP