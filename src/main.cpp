//#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "Window.hpp"
#include <iostream>
#include<memory>

int main(int argv, char** args)
{
    int initCode = SDL_Init(SDL_INIT_EVERYTHING);
    if(initCode!=0){
        std::cout<<"Init fail: "<<SDL_GetError()<<std::endl;
    }
    std::string windowName = "Test";
    std::unique_ptr<Window> mainWindow = std::make_unique<Window>(windowName);
    mainWindow->m_Init();
    SDL_Delay(10000);
    SDL_Quit();
    return 0;
}