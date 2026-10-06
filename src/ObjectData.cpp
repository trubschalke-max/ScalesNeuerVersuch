#include "ObjectData.hpp"

using ObjectData::Activeness;

bool ObjectData::m_meansHoveredOver(Activeness ac){
    return (int)ac & (int)Activeness::HOVERED_OVER == (int)Activeness::HOVERED_OVER;
}