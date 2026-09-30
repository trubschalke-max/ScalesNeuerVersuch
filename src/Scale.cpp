#include "Scale.hpp"
#include "ScaleElement.hpp"

Scale::Scale(std::string above, std::string below, Svalue selection): m_data{above, below, selection}
{}

Scale::Scale(const ScaleData& data): m_data(data){}

Scale::~Scale(){}

std::string Scale::m_textAt(Svalue where){
    int iwhere = (int) where, imax = (int) m_data.maxValue, imin = (int) m_data.minValue;
    
    if(where > imax || where < imin - FURTHER_SCALE_BUTTONS_COUNT){
        return "";
    }
    if(where == m_scaleValueForCommitting())
        return "Commit!";
    if(where == m_scaleValueForUndoing())
        return "Undo!";

    std::string firstHalf = std::to_string(iwhere) + ": ", secondHalf = "";
    
    if(iwhere == imax)
        secondHalf = m_data.above;
    if(iwhere == imin)
        secondHalf = m_data.below;

    return firstHalf + secondHalf;
}

bool Scale::m_contains(ScaleValue sv){
    int imax = (int) m_data.maxValue, imin = (int) m_data.minValue, isv = (int) sv;
    if(imax>= isv && imin <= isv)
        return true;
    else return false;
}

ErrorCode Scale::m_commit(){
    switch(m_data.state){
        case COMMITTED: return ErrorCode::ALREADY_COMMITTED;
        case NOT_YET_COMMITTED:
        if(!m_contains(m_data.selection)){
            if(m_data.selection == SCALE_NO_VALUE){
                return ErrorCode::INVALID_COMMIT;
            }
        }
            m_data.state = COMMITTED;
            return ErrorCode::SUCCESS;
        case ERROR:
            return ErrorCode::FAILURE;
        default: throw std::runtime_error("Inexplicable scale value while committing");
    }
}

ErrorCode Scale::m_undo(){
    switch(m_data.state){
        case COMMITTED: return ErrorCode::ALREADY_COMMITTED;
        case NOT_YET_COMMITTED:
            if(m_contains(m_data.selection)){
                m_data.selection = SCALE_NO_VALUE;
                return ErrorCode::SUCCESS;
            }
            return ErrorCode::INVALID_REDOING;
        case ERROR:
            return ErrorCode::FAILURE;
        default: throw std::runtime_error("Inexplicable scale value while undoing");
    }
}

ErrorCode Scale::m_select(ScaleValue newVal){
    switch(m_data.state){
    case COMMITTED: return ErrorCode::ALREADY_COMMITTED;
    case NOT_YET_COMMITTED:
        if(m_contains(newVal)){
            m_data.selection = newVal;
            return ErrorCode::SUCCESS;
        }
        return ErrorCode::INVALID_SCALE_SELECTION;
    case ERROR:
        return ErrorCode::FAILURE;
    default: throw std::runtime_error("Inexplicable value in the scale before undoing");
    }
}

std::string toString(Scale::ScaleValue val){
    if(val == Scale::SCALE_NO_VALUE){
        return "SCALE_NO_VALUE";
    }
    if(val == Scale::DUMMY_VALUE){
        return "SCALE_DUMMY_VALUE";
    }
    std::string str = "SCALE_" + std::to_string((int) val);
}