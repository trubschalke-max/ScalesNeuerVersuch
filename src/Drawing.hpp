#ifndef DRAWING_HPP
#define DRAWING_HPP

#include<list>
#include "ErrorCode.hpp"
#include "SDL2/SDL.h"
#include "DrawingObject.hpp"

class GameObject;

class Drawing: public DrawingObject{
private: 
    SDL_Renderer* m_renderer;
    std::list<GameObject*> m_queue; //high layers at the end
protected:    
    const auto& m_Queue(){return m_queue;}
    EventAcceptance m_acceptsEvents = EventAcceptance::NONE;
public:
    Drawing(SDL_Renderer* renderer);
    virtual ~Drawing();
    ErrorCode m_draw() override;
    ErrorCode m_add(GameObject*);
    ErrorCode m_remove(GameObject*);
    SDL_Renderer* m_Renderer(){return m_renderer;}
    virtual ErrorCode m_handleEvent(const SDL_Event& event, ErrorList whatCanIIgnore = ErrorHandling::standardErrorHierarchy()) override; //The default behavior is to return NOTHING_HAPPENED; or if events are accepted, to backwardsly iterate through the queue and stop when the first object has not returned NOTHING_HAPPENED.
    EventAcceptance m_AcceptsEvents(){return m_acceptsEvents;}
    ErrorCode m_renderPresent() override;

    void m_clear(){m_queue.clear();}
    void m_setRenderer(SDL_Renderer* renderer){m_renderer = renderer;}
    void m_setAcceptsEvents(EventAcceptance ea){m_acceptsEvents = ea;}
    const Suggestor::Suggestion*m_retrieveSuggestion() override; //In case there was no nonempty suggestion, nullptr is returned.
};

#endif // DRAWING_HPP