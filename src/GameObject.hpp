#ifndef GAME_OBJECT
#define GAME_OBJECT

#include<SDL2/SDL.h>
#include "ErrorCode.hpp"



class GameObject{
public:
    enum Layer{DEFAULT_LAYER = 0};
protected: 
    int m_x;
    int m_y;
    Layer m_layer;
    bool m_isClickable = false;
    virtual ErrorCode m_leftClick(int x, int y);
    ErrorCode m_onlyHandleLeftClicks(const SDL_Event& e); //The purpose is to check if it was hit by a left click; and then redirect to m_leftClick(). If not hit, the return value is NOT_ACTUALLY_CLICKED. 
    //The default behavior of m_leftClick() is to return ErrorCode::NOTHING_HAPPENED. 

public:
    GameObject(int x, int y, Layer layer = DEFAULT_LAYER);
    virtual ErrorCode m_draw(SDL_Renderer* renderer) = 0;
    virtual bool m_isHitBy(int x, int y) = 0;
    virtual ErrorCode m_handleEvent(const SDL_Event& e); //The default behavior is to call m_onlyHandleLeftClicks().
    int m_X(){return m_x;}
    int m_Y(){return m_y;}
    int m_Layer(){return m_layer;}
    bool m_IsClickable(){return m_isClickable;}
};

#endif