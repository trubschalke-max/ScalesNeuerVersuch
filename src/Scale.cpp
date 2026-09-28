#include "Scale.hpp"

Scale::Scale(std::string above, std::string below, Svalue selection): m_data{above, below, selection}
{}

Scale::Scale(const ScaleData& data): m_data(data){}

Scale::~Scale(){}

std::string Scale::m_textAt(Svalue where){
    int iwhere = (int) where, imax = (int) m_data.maxValue, imin = (int) m_data.minValue;
    
    if(where > imax || where < imin){
        return "";
    }

    std::string firstHalf = std::to_string(iwhere) + ": ", secondHalf = "";
    
    if(iwhere == imax)
        secondHalf = m_data.above;
    if(iwhere == imax)
        secondHalf = m_data.below;

    return firstHalf + secondHalf;
}