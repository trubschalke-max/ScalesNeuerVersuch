//#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "Window.hpp"
#include "Graphics.hpp"
#include <iostream>
#include<memory>
#include "ClassicUnhoverableButtonElement.hpp"
#include "Game.hpp"

int main(int argv, char** args)
{
   Game game;
    return (int) game.m_simpleTest();
}