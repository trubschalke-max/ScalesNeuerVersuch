#include "EventHandler.hpp"
#include "myExceptions.hpp"

typedef EventHandler::HandlingClass HC;

EventHandler::EventHandler(Drawing* whatToDraw): m_whatToDraw(whatToDraw){}
EventHandler::~EventHandler(){}
Displayer::Displayer(Drawing* whatToDraw): EventHandler(whatToDraw){if(!whatToDraw) throw std::runtime_error("Displayer constructed without a drawing");}
Displayer::~Displayer(){}

bool EventHandler::m_isQuit(const SDL_Event& event){
    if(event.type == SDL_QUIT)
        return true;
    return false;
}

ErrorCode EventHandler::m_enter(){
    ErrorCode whatToReturn = ErrorCode::OK;
    if(m_whatToDraw){
        checkForError(m_whatToDraw->m_draw(), whatToReturn);
        SDL_RenderPresent(m_whatToDraw->m_Renderer());
    }
    return whatToReturn;
}

ErrorCode EventHandler::m_eventLoop(){
try{
    ErrorCode error = ErrorCode::OK;
    checkForError(m_enter(), error);
    SDL_Event event;
    bool quit = false;
    while(!quit){
        if(SDL_PollEvent(&event)){
            if(m_isQuit(event)){
                throw SDLQuitException();
            }
            HandlingClass diagnosis = m_classify(event);
            HandlingResult result = HandlingResult::NO_RESULT_YET;
            switch(diagnosis){
                case IRRELEVANT: break;
                case HC_QUIT_SDL: throw SDLQuitException();
                case HC_QUIT_LOOP: 
                quit = true; 
                break;
                default:{
                    result = m_handleRelevantEvent(event, diagnosis);
                    checkForError(m_handleResult(result), error);
                    switch(result){
                        case HandlingResult::QUIT_FROM_SDL: throw SDLQuitException();
                        case HandlingResult::QUIT_LOOP: 
                            quit = true;
                            default: 
                            break;
                        }
                    }
                }
            }
            SDL_Delay(m_delayBetweenIterations); //This reduces energy consumption
        }
        return error;
    }
    catch(...){
        throw;
    }
}
    
    ErrorCode EventHandler::m_handleResult(EventHandler::HandlingResult result){
        ErrorCode error = ErrorCode::OK;
        switch(result){
            case HandlingResult::REDRAW:
            if(!m_whatToDraw){
                return ErrorCode::NULL_DRAWING_IN_EVENT_HANDLER;
            }
            checkForError(m_whatToDraw->m_draw(), error);
            SDL_RenderPresent(m_whatToDraw->m_Renderer());
        default:
            return ErrorCode::OK;
    }
}

HC EventHandler::m_classify(const SDL_Event& event){
    return UNSPECIFIED;
}


HC Displayer::m_classify(const SDL_Event& event){
    if(event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
        return HC_QUIT_LOOP; 
    }
    else return IRRELEVANT;
}

EventHandler::HandlingResult Displayer::m_handleRelevantEvent(const SDL_Event& event, HandlingClass diagnosis){
    if(diagnosis != HC_QUIT_LOOP){
        throw InvalidHandlingClass(diagnosis, this);
    }
    return HandlingResult::QUIT_LOOP;
}

std::string EventHandler::m_classDescription(){return "BasicEventHandler";}
std::string Displayer::m_classDescription(){return "Displayer";}

std::string toString(EventHandler::HandlingClass hc){
    switch(hc){
        case HC::UNSPECIFIED: return "UNSPECIFIED";
        case HC::IRRELEVANT: return "IRRELEVANT";
        case HC::HC_QUIT_SDL: return "HC_QUIT_SDL";
        case HC::HC_QUIT_LOOP: return "HC_QUIT_LOOP";
        case HC::KEYDOWN: return "KEYDOWN";
        case HC::LEFT_CLICK: return "LEFT_CLICK";
        case HC::RIGHT_CLICK: return "RIGHT_CLICK";
        case HC::MOUSE_MOTION: return "MOUSE_MOTION";
        default: std::string tmp = "HANDLING_CLASS_NUMBER: ";
        tmp.append(std::to_string((int) hc));
        return tmp.c_str();
    }
}
std::ostream& operator<<(std::ostream& os, const EventHandler::HandlingClass hc){return os<<toString(hc);}

