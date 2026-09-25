#include "Game.hpp"
#include "SDL2/SDL.h"
#include "Window.hpp"
#include "Graphics.hpp"
#include <iostream>
#include<memory>
#include "ClassicUnhoverableButtonElement.hpp"


Game::Game()
{
    m_init();
}

Game::~Game()
{
    m_mainWindow.reset(0);
    m_theFont.reset(0);
    TTF_Quit();
    SDL_Quit();
}

void Game::m_init(){
    int initCode = SDL_Init(SDL_INIT_EVERYTHING);
    if(initCode!=0){
        std::cout<<"Init fail: "<<SDL_GetError()<<std::endl;
    }
    initCode = TTF_Init();
    if(initCode!=0){
        std::cout<<"TTF_Init fail: "<<SDL_GetError()<<std::endl;
    }    
    const std::string windowName = "Test";
    const std::string fontFilename = {"build/Fonts/FT88-Regular.ttf"};
    m_theFont = std::make_unique<Graphics::Font>(fontFilename);
    Graphics::theFont = m_theFont.get();

    m_mainWindow = std::make_unique<Window>(windowName);
    m_mainWindow->m_init();
}

ErrorCode Game::m_simpleTest(){
    std::unique_ptr<Button> myButton= std::make_unique<Button>(0, 30, 100, 100, "Testknopf", Button::ColorSet{Graphics::blackColor(), Graphics::whiteColor(), Graphics::yellowColor()});
    m_mainWindow->m_Drawing().m_add(myButton.get());  
    ErrorCode error = m_mainWindow->m_Drawing().m_draw();
    SDL_RenderPresent(m_mainWindow->m_Renderer());
    std::cout<<error<<std::endl;
    return error;
}