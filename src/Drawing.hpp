#ifndef DRAWING_QUEUE
#define DRAWING_QUEUE

#include<list>
#include "ErrorCode.hpp"
#include "SDL2/SDL.h"
class GameObject;

class Drawing{
public: enum class EventAcceptance{NONE, ONE, MANY};
private: 
    SDL_Renderer* m_renderer;
    std::list<GameObject*> m_queue; //high layers at the end
protected:    
    const auto& m_Queue(){return m_queue;}
    EventAcceptance m_acceptsEvents = EventAcceptance::NONE;
public:
    Drawing(SDL_Renderer* renderer);
    virtual ~Drawing();
    ErrorCode m_draw();
    ErrorCode m_add(GameObject*);
    ErrorCode m_remove(GameObject*);
    SDL_Renderer* m_Renderer(){return m_renderer;}
    virtual ErrorCode m_handleEvent(const SDL_Event& event, ErrorList whatCanIIgnore = ErrorHandling::standardErrorHierarchy()); //The default behavior is to return NOTHING_HAPPENED; or if events are accepted, to backwardsly iterate through the queue and stop when the first object has not returned NOTHING_HAPPENED.
    EventAcceptance m_AcceptsEvents(){return m_acceptsEvents;}
    void m_setAcceptsEvents(EventAcceptance ea){m_acceptsEvents = ea;}
};

#endif