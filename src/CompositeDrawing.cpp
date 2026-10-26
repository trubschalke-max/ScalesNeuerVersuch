#include "CompositeDrawing.hpp"
#include "Drawing.hpp"
#include <iostream>
#include "SDL2/SDL.h"
#include "sdltypes.h"

using ErrorHandling::checkForErrorStandardHierarchy;


CompositeDrawing::CompositeDrawing()
{

}

CompositeDrawing::~CompositeDrawing()
{

}

ErrorCode CompositeDrawing::m_draw(std::vector<int>& playerIDs){
    ErrorCode error = ErrorCode::OK;
    for(auto& playerID : playerIDs){
        if(m_drawings.find(playerID) == m_drawings.end()){
            checkForErrorStandardHierarchy(ErrorCode::INVALID_DRAWING_ID, error);
            std::cerr<< "Invalid drawing ID: " << playerID << std::endl;
        }
        checkForErrorStandardHierarchy(m_drawings[playerID]->m_draw(), error);
    }
    return error;
}

ErrorCode CompositeDrawing::m_draw(){
    ErrorCode error = ErrorCode::OK;
    std::vector<int> playerIDs;
    for (const auto& pair : m_drawings) {//Somehow, playerID 0 wasnt found
        playerIDs.push_back(pair.first);
    }
    checkForErrorStandardHierarchy(m_draw(playerIDs), error);
    return error;
}
    
ErrorCode CompositeDrawing::m_add(Drawing* drawing, int playerID){
    ErrorCode error = ErrorCode::OK;
    if(m_drawings.find(playerID) != m_drawings.end()){
        checkForErrorStandardHierarchy(ErrorCode::DUPLICATE_DRAWING_ID, error);
        std::cerr<< "Drawing ID already exists: " << playerID << std::endl;
    } else {
        m_drawings[playerID] = drawing;
    }
    return error;
}

ErrorCode CompositeDrawing::m_remove(Drawing* drawing, int playerID){
    ErrorCode error = ErrorCode::OK;
    auto it = m_drawings.find(playerID);
    if(it == m_drawings.end()){
        checkForErrorStandardHierarchy(ErrorCode::INVALID_DRAWING_ID, error);
        std::cerr<< "Drawing ID to delete does not exist: " << playerID << std::endl;
    } else if(it->second != drawing){
        checkForErrorStandardHierarchy(ErrorCode::INVALID_DRAWING_ID, error);
        std::cerr<< "Drawing pointer does not match for ID: " << playerID << std::endl;
    } else {
        m_drawings.erase(it);
    }
    return error;
}

ErrorCode CompositeDrawing::m_handleEvent(const SDL_Event& event, ErrorList whatCanIIgnore){//Right now, it is assumed that any event does only apply to one drawing at a time. I might later enforce this by an event acceptance member.
try{
    ErrorCode error = ErrorCode::NOTHING_HAPPENED;
    for(auto& pair : m_drawings){
        Uint32 windowID = SDL_GetWindowIDFromEvent(event);
        if(windowID == -1){
            throw std::logic_error("Unsupported SDL event supposed to be handled by CompositeDrawing.");
        }
        // Check if the event belongs to this window before passing it to the child drawing. (I forgot this!!!)
        SDL_Window* window = SDL_RenderGetWindow(pair.second->m_Renderer());    
        if(SDL_GetWindowID(window) != windowID){
            continue;
        }
        ErrorCode childError = pair.second->m_handleEvent(event, whatCanIIgnore);
        if(childError == ErrorCode::DRAWING_MUST_NOT_HANDLE_EVENTS){
            return ErrorCode::DRAWING_MUST_NOT_HANDLE_EVENTS;
        }
        checkForErrorStandardHierarchy(childError, error);
    }
    return error;
} catch (const UnsupportedSDLEvent& e) {
    std::cerr << e.what() << std::endl;
    return ErrorCode::EVENT_HANDLING_NOT_REDIRECTED;
}
}

ErrorCode CompositeDrawing::m_renderPresent(){
    ErrorCode error = ErrorCode::OK;
    for(auto& pair : m_drawings){
        checkForErrorStandardHierarchy(pair.second->m_renderPresent(), error);
    }
    return error;
}
void CompositeDrawing::m_setAcceptsEvents(EventAcceptance ea){
    for(auto& pair : m_drawings){
        pair.second->m_setAcceptsEvents(ea);
    }
}

const Suggestor::Suggestion*CompositeDrawing::m_retrieveSuggestion(){
    for(auto& pair : m_drawings){
        const Suggestor::Suggestion* suggestion = pair.second->m_retrieveSuggestion();
        if(suggestion != nullptr){
            return suggestion;
        }
    }
    return nullptr;
}

ErrorCode CompositeDrawing::m_resetSuggestion(){
    ErrorCode error = ErrorCode::OK;
    for(auto& pair : m_drawings){
        checkForErrorStandardHierarchy(pair.second->m_resetSuggestion(), error);
    }
    return error;
}

ErrorCode CompositeDrawing::m_handleValidSuggestion(const Suggestor::Suggestion& suggestion){//This makes me regret that drawings are not composite drawings.
  ErrorCode error = ErrorCode::NOTHING_HAPPENED;
    int okCounter = 0;
    for(auto& pair: m_drawings){
        ErrorCode rawEC = pair.second->m_handleValidSuggestion(suggestion);
        if(rawEC == ErrorCode::OK)
            okCounter++;
        ErrorHandling::checkForErrorStandardHierarchy(rawEC, error);
    }
    if(error != ErrorCode::NOTHING_HAPPENED && error != ErrorCode::OK)
        return error;
    if(suggestion.applicability != Suggestor::Suggestion::ApplicabilityType::MULTIPLE && suggestion.applicability != Suggestor::Suggestion::ApplicabilityType::ERROR && okCounter > (int) suggestion.applicability){
        throw std::runtime_error("Multiple handlings of suggestion detected");
        error = ErrorCode::MULTIPLE_HANDLINGS_OF_SUGGESTION;
    }
    if(okCounter == 0)
        error = ErrorCode::NOTHING_HAPPENED; //This should not happen.
    if(okCounter == 1)
        error = ErrorCode::OK;
    return error;
}

ErrorCode CompositeDrawing::m_updateDisplayingEverywhere(){
    ErrorCode error = ErrorCode::OK;
    for(auto& pair : m_drawings){
        checkForErrorStandardHierarchy(pair.second->m_updateDisplayingEverywhere(), error);
    }
    return error;
}