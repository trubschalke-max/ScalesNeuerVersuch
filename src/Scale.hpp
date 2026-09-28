#ifndef SCALE_HPP
#define SCALE_HPP
#include<string>
#include"ErrorCode.hpp"
#include<map>
#include<SDL2/SDL.h>
#include "GameObject.hpp"

class Scale{
public: 
    //enum class State{PUBLISHED, WORK_IN_PROGRESS}; //I do not like the name WORK_IN_PROGRESS.
    enum ScaleValue{SCALE_NO_VALUE = -1, SCALE_0, SCALE_1, SCALE_2, SCALE_3, SCALE_4, SCALE_CANONICAL_END = 4,
    NOT_CLICKED = -2, CLICKING_CONFLICT = -3, DUMMY_VALUE = -4
    };
    struct ScaleData{
        std::string above;
        std::string below;
        ScaleValue selection;
        ScaleValue minValue = SCALE_0;
        ScaleValue maxValue = SCALE_CANONICAL_END;
        //State state = State::WORK_IN_PROGRESS;
    };
private:
    std::map<SDL_Renderer*, std::map<ScaleValue, SDL_Rect>> m_currentButtons;
    ScaleData m_data;

    
    public:
    Scale(std::string above, std::string below, ScaleValue selection = SCALE_NO_VALUE);
    Scale(const ScaleData& data);
    virtual ~Scale();
    const ScaleData& m_Data(){return m_data;}
    
    ErrorCode m_drawScale(int x, int y, SDL_Renderer* renderer, SDL_Rect& output);
    ScaleValue m_whereClicked(int x, int y, SDL_Renderer* renderer);
    ScaleValue m_select(ScaleValue newValue); 
    ErrorCode m_commit(); //This will make the current selection final. Most notably, the state will be set to PUBLISHED.    
    std::string m_textAt(ScaleValue nr);
};

typedef Scale::ScaleValue Svalue;


#endif // SCALE_HPP