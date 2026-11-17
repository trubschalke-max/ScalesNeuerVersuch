#ifndef PLAYERUI_H
#define PLAYERUI_H
#pragma once

#include <string>
#include "Drawing.hpp"
#include <memory>
#include "Window.hpp"

class Player;

class PlayerUI
{
private:
    Player* m_associatedPlayer;
    Drawing m_associatedDrawing; //The drawings are not meant to be accessed this way. Instead, they shall be accessed by EventHandlers. The drawings will be passed to the game, which will in turn pass bind them to the event handlers.
    Window m_associatedWindow;
    std::string m_playerName;
    int m_nextScaleElementX = 0; // Keeps track of the next X position for scale elements.
public:
    PlayerUI(std::string playerName, Player* player);//The UI's constructor initializes the associated drawing and window for the player.
    SDL_Point m_figureOutPosition(GameObject* obj, ObjectData::GameObjectType type, bool remember = true); //This function will, if I don't engage in making further graphics, sort the scale elements from left to right and put the explainableset element below. If remember is set, it will update the next position for scale elements. (Possibly, even more in the future.)
    ~PlayerUI();
    friend class Player;
    void m_clear(); // Clears the associated drawing and resets the next scale element position.
    void m_resetRoundBasedShifts(); // Resets the next scale element position for the new round.
};

#endif