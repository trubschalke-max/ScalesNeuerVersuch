#include "ObjectData.hpp"

using ObjectData::Activeness;

bool ObjectData::m_meansHoveredOver(Activeness ac){
    return (int)ac & (int)Activeness::HOVERED_OVER == (int)Activeness::HOVERED_OVER;
}

std::string toString(ObjectData::Activeness ac){
    switch(ac){
        case Activeness::ACTIVE: return "ACTIVE";
        case Activeness::HOVERED_OVER: return "HOVERED_OVER";
        case Activeness::HOVERED_OVER_AND_ACTIVE: return "HOVERED_OVER_AND_ACTIVE";
        case Activeness::PASSIVE: return "PASSIVE";
        case Activeness::FORBIDDEN: return "FORBIDDEN";
        case Activeness::PERMANENTLY_SELECTED: return "PERMANENTLY_SELECTED";
        case Activeness::ACTIVENESS_ERROR: return "ACTIVENESS_ERROR";
        case Activeness::SECRET: return "SECRET";
        case Activeness::SECRETLY_ACTIVE: return "SECRETLY_ACTIVE";
    }
    return "UNKNOWN_ACTIVENESS: "+std::to_string((int)ac);
}

std::string toString(ObjectData::WordState ws){
    switch(ws){
        case ObjectData::WordState::WRONG: return "WRONG";
        case ObjectData::WordState::CORRECT: return "CORRECT";
        case ObjectData::WordState::TRIED: return "TRIED";
        case ObjectData::WordState::CORRECTLY_TRIED: return "CORRECTLY_TRIED";
    }
    return "UNKNOWN_WORDSTATE: "+std::to_string((int)ws);
}

ObjectData::Activeness operator|(ObjectData::Activeness a, ObjectData::Activeness b){
    return static_cast<ObjectData::Activeness>(static_cast<int>(a) | static_cast<int>(b));
}

ObjectData::WordState operator|(ObjectData::WordState a, ObjectData::WordState b){
    return static_cast<ObjectData::WordState>(static_cast<int>(a) | static_cast<int>(b));
}

ObjectData::Activeness operator&(ObjectData::Activeness a, ObjectData::Activeness b){
    return static_cast<ObjectData::Activeness>(static_cast<int>(a) & static_cast<int>(b));
}

ObjectData::WordState operator&(ObjectData::WordState a, ObjectData::WordState b){
    return static_cast<ObjectData::WordState>(static_cast<int>(a) & static_cast<int>(b));
}