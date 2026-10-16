#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#pragma once

#include "Window.hpp"
#include "myExceptions.hpp"


class EventHandler
{
public:
    enum HandlingClass{
        IRRELEVANT, UNSPECIFIED, HC_QUIT_SDL, HC_QUIT_LOOP, LEFT_CLICK, RIGHT_CLICK, MOUSE_MOTION, KEYDOWN
    };
    enum class HandlingResult{
        GO_ON, REDRAW = 1, QUIT_FROM_SDL = 2, QUIT_LOOP = 4, NO_RESULT_YET = 8, FAILURE = 16, QUIT_AND_REDRAW = QUIT_LOOP | REDRAW
    };
    
    protected:
    DrawingObject* m_whatToDraw;
    Uint32 m_delayBetweenIterations = 100;
    
    virtual HandlingResult m_handleRelevantEvent(const SDL_Event& event, HandlingClass diagnosis) = 0;
    virtual HandlingClass m_classify(const SDL_Event& event); //For preprocessing. The default behavior is to mark everything as unspecified.
    virtual bool m_isQuit(const SDL_Event& event); //only to filter the most obvious of quits
    virtual ErrorCode m_handleResult(HandlingResult hr); //The default behavior is to draw if drawing is not NULL and the redraw flag is set. Possible error return: NULL_DRAWING_IN_EVENT_HANDLER. 
    virtual ErrorCode m_enter(); //The default behavior is having the drawing draw and renderPresent once
    
    public:
    EventHandler(DrawingObject* whatToDraw);
    EventHandler() = delete;
    virtual ~EventHandler();
    
    ErrorCode m_eventLoop();
    
    virtual std::string m_classDescription(); //only for exceptions
    Uint32 m_DelayBetweenIterations(){return m_delayBetweenIterations;}
    void m_setDelayBetweenIterations(Uint32 dbi){m_delayBetweenIterations = dbi;}
};
typedef EventHandler BasicEventHandler;

class Displayer: public EventHandler{
protected: 
    virtual HandlingResult m_handleRelevantEvent(const SDL_Event& event, HandlingClass diagnosis);
    virtual HandlingClass m_classify(const SDL_Event& event);
    virtual std::string m_classDescription();
    static ErrorCode s_displayText(Window* window); //this shall correspond to the legacy code
public:
    Displayer(DrawingObject* whatToDraw);
    Displayer() = delete;
    virtual ~Displayer();
};

std::string toString(EventHandler::HandlingClass hc);
std::ostream& operator<<(std::ostream& os, const EventHandler::HandlingClass& hc);

class InvalidHandlingClass: public std::exception{
public: 
    EventHandler::HandlingClass hc;
    EventHandler* eh;
    InvalidHandlingClass(EventHandler::HandlingClass phc, EventHandler* peh):hc(phc), eh(peh){}
    const char* what(){std::string whatToReturn; whatToReturn = "Invalid handling Class: "+eh->m_classDescription()+" for "+toString(hc); return whatToReturn.c_str();}
};



#endif