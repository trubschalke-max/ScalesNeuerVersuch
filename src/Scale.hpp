#ifndef SCALE_HPP
#define SCALE_HPP
#include<string>
#include"ErrorCode.hpp"
#include<map>
#include<SDL2/SDL.h>
#include "GameObject.hpp"
#include "ObjectData.hpp"

class Scale{
public: 
private:
    std::map<SDL_Renderer*, std::map<ObjectData::ScaleValue, SDL_Rect>> m_currentButtons;
    ObjectData::ScaleData m_data;

    
    public:
    Scale(std::string above, std::string below, ObjectData::ScaleValue selection = ObjectData::ScaleValue::SCALE_NO_VALUE);
    Scale(const ObjectData::ScaleData& data);
    virtual ~Scale();
    const ObjectData::ScaleData& m_Data(){return m_data;}
    
    ErrorCode m_select(ObjectData::ScaleValue newValue); 
    ErrorCode m_commit(); //This will make the current selection final. Most notably, the state will be set to COMMITTED. In case it was committed already, it returns ALREADY_COMMITTED. In case nothing was selected, it returns INVALID_COMMIT. If state was ERROR  before, FAILURE is returned.
    ErrorCode m_undo(); //This will undo the previous selection.In case it was committed already, it returns ALREADY_COMMITTED. In case nothing was selected, it returns INVALID_COMMIT. If state was ERROR  before, FAILURE is returned.
    std::string m_textAt(ObjectData::ScaleValue nr);
    ObjectData::ScaleValue m_scaleValueForCommitting(){return (ObjectData::ScaleValue) ((int) m_data.minValue - 1);}
    ObjectData::ScaleValue m_scaleValueForUndoing(){return (ObjectData::ScaleValue) ((int) m_data.minValue - 2);}
    bool m_contains(ObjectData::ScaleValue sv); //Returns true on any value between the minimum and maximum of the regular scale values (excluding further buttons)

    ErrorCode m_setState(ObjectData::State newState); //Sets the state of the scale.
};

typedef ObjectData::ScaleValue Svalue;


#endif // SCALE_HPP