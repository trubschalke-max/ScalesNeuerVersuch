#include "SDL2/SDL.h"
#include "myExceptions.hpp"


Uint32 SDL_GetWindowIDFromEvent(const SDL_Event& event){//Note that this function throws UnsupportedSDLEvent for unsupported event types. This may include any event type not related to a window, no matter if the SDL_Event is supported by the caller.
    switch(event.type){
        case SDL_WINDOWEVENT:
            return event.window.windowID;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return event.key.windowID;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            return event.button.windowID;
        case SDL_MOUSEMOTION:
            return event.motion.windowID;
        default:
            throw UnsupportedSDLEvent(event); // Invalid or unsupported event type
    }
}