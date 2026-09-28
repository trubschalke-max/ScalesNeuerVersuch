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
    int m_layer;

public:
    GameObject(int x, int y, Layer layer = DEFAULT_LAYER);
    virtual ErrorCode m_draw(SDL_Renderer* renderer) = 0;
    virtual bool m_isHitBy(int x, int y) = 0;
    int m_X(){return m_x;}
    int m_Y(){return m_y;}
    int m_Layer(){return m_layer;}
};

#endif