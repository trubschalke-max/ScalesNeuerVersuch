//#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "Window.hpp"
#include "Graphics.hpp"
#include <iostream>
#include<memory>
#include "ClassicUnhoverableButtonElement.hpp"
#include "Game.hpp"

int main(int argv, char** args)
{//Note to myself: A lot of invalid suggestions shall be treated like IGNORE_THIS.
//Second note to myself: When initializing the round, pass the game objects to the players (and create them)
    Game game;
    return (int) game.m_simpleTest();
}