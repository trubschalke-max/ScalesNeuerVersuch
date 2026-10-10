#include "Game.hpp"
#include "SDL2/SDL.h"
#include "Window.hpp"
#include "Graphics.hpp"
#include <iostream>
#include<memory>
#include "ClassicUnhoverableButtonElement.hpp"
#include "ScaleElement.hpp"
#include "BasicGameObjectHandler.hpp"
#include "ExplainableSetElement.hpp"
#include <numeric>

using ObjectData::ScaleValue;

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
    try{

        Scale scale("above", "below", ScaleValue::SCALE_1);
        ScaleElement element(0, 100, &scale);
        std::vector<int> words(24);
        std::iota(words.begin(), words.end(), 0);

        std::vector<std::string> wordStrings;
        for(int i(0); i < words.size(); ++i){
            wordStrings.push_back(std::to_string(words[i]));
        }
        ExplainableSet explainables(wordStrings, ObjectData::ExplainableSetValue::EXSET_1);
        ExplainableSetElement exel(500, 500, &explainables);
        Drawing drawing(m_mainWindow->m_Renderer());
        drawing.m_add(&element);
        drawing.m_add(&exel);
        BasicGameObjectHandler e(&drawing);
        return e.m_eventLoop();
    }
    catch(SDLQuitException& e){
        return e.m_error;
    }
}
