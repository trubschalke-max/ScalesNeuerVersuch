#ifndef GAME_H
#define GAME_H

#include "Window.hpp"
#include "Graphics.hpp"
#include <memory>

#pragma once

class Game
{

private:
    std::unique_ptr<Window> m_mainWindow;
    std::unique_ptr<Graphics::Font> m_theFont;
    void m_init();
    public:
    ErrorCode m_simpleTest();
    Game();
    virtual ~Game();

};

#endif