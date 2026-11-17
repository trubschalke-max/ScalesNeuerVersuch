#ifndef PLAYERSTATEINDICATINGELEMENTDISPLAYINGSUPPORTER_H
#define PLAYERSTATEINDICATINGELEMENTDISPLAYINGSUPPORTER_H
#pragma once

#include "SuggestSupport/HoverableCompositeButtonElementDisplayingSupporter.hpp"

class PlayerStateIndicatingElement;

class PlayerStateIndicatingElementDisplayingSupporter: public HoverableCompositeButtonElementDisplayingSupporter
{
private:
    PlayerStateIndicatingElement* m_playerStateIndicatingElement;
public:
    PlayerStateIndicatingElementDisplayingSupporter(PlayerStateIndicatingElement* element);
    ~PlayerStateIndicatingElementDisplayingSupporter();
    ErrorCode m_updateDisplaying();

};

#endif