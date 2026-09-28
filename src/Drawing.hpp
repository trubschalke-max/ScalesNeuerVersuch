#ifndef DRAWING_QUEUE
#define DRAWING_QUEUE

#include<list>
#include "ErrorCode.hpp"
#include "SDL2/SDL.h"
class GameObject;

class Drawing{
private:    
    std::list<GameObject*> m_queue; //high layers at the end
    SDL_Renderer* m_renderer;
public:
    Drawing(SDL_Renderer* renderer);
    ErrorCode m_draw();
    ErrorCode m_add(GameObject*);
    ErrorCode m_remove(GameObject*);
    SDL_Renderer* m_Renderer(){return m_renderer;}
};

#endif