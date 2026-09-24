#ifndef SCALE_HPP
#define SCALE_HPP
#include<string>
#include"ErrorCode.hpp"
#include<map>
#include<SDL2/SDL.h>

class Scale{
public:
    enum class State{PUBLISHED, WORK_IN_PROGRESS}; //I do not like the name WORK_IN_PROGRESS.
    enum ScaleValue{SCALE_NO_VALUE = -1, SCALE_0, SCALE_1, SCALE_2, SCALE_3, SCALE_4, SCALE_CANONICAL_END = 4,
    NOT_CLICKED = -2, CLICKING_CONFLICT = -3, DUMMY_VALUE = -4
    };
    struct ScaleData{
        std::string m_above;
        std::string m_below;
        ScaleValue m_selection;
        ScaleValue SCALE_BELOW = SCALE_0;
        ScaleValue SCALE_ABOVE = SCALE_CANONICAL_END;
        State m_state = State::WORK_IN_PROGRESS;
    };
private:
    std::map<SDL_Renderer*, std::map<ScaleValue, SDL_Rect>> m_currentButtons;
    ScaleData m_data;

    std::string m_textAt(ScaleValue nr);

public:
    Scale(std::string above, std::string below, ScaleValue selection = SCALE_NO_VALUE);
    Scale(const ScaleData& data);
    virtual ~Scale();
    const ScaleData& m_Data(){return m_data;}
    
    ErrorCode m_drawScale(int x, int y, SDL_Renderer* renderer, SDL_Rect& output);
    ScaleValue m_whereClicked(int x, int y, SDL_Renderer* renderer);
    ScaleValue m_select(ScaleValue newValue); 
    ErrorCode m_commit(); //This will make the current selection final. Most notably, the state will be set to PUBLISHED.    

};



#endif // SCALE_HPP