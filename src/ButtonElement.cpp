#include "ButtonElement.hpp"
#include "Graphics.hpp"
#include <iostream>
#include <vector>

ButtonElement::ButtonElement(int x, int y, int width, int height, std::string text, GameObject::Layer layer, ObjectData::Activeness activeness): GameObject(x, y, layer), m_width(width), m_height(height), m_textDisplayed(text), m_activeness(activeness)
{

}

ButtonElement::ButtonElement(SDL_Rect shapeAndPlace, std::string text,GameObject::Layer layer, ObjectData::Activeness activeness):ButtonElement(shapeAndPlace.x, shapeAndPlace.y, shapeAndPlace.w, shapeAndPlace.h, text, layer, activeness){}

ButtonElement::~ButtonElement()
{

}

ErrorCode ButtonElement::m_draw(SDL_Renderer* renderer){//Better redo this. In particular, make sure the sdl rect has the right size.
    ErrorCode error = ErrorCode::OK;
    SDL_Rect rectOfSurfaces;
    std::vector<SDL_Surface*> textSurfaces = Graphics::createSurfacesFromString(m_textDisplayed, m_currentTextColor(), renderer, &rectOfSurfaces);    
    if(rectOfSurfaces.w == 0 || rectOfSurfaces.h == 0)
        error = ErrorCode::INVALID_RECT;
    if(rectOfSurfaces.w > m_width || rectOfSurfaces.h > m_height)
        error = ErrorCode::TOO_BIG_RECT;
    //Fill the background
    checkForError(Graphics::FillRect(m_areaRect(), m_currentBackgroundColor(), renderer), error);

    int scaleFactorForText = std::min(m_width/rectOfSurfaces.w, m_height/rectOfSurfaces.h);
    int currentY = m_y;
    for(SDL_Surface* currentTextSurface: textSurfaces){
        SDL_Texture* currentTextTexture = SDL_CreateTextureFromSurface(renderer, currentTextSurface);
        SDL_Rect currentRect{m_x, currentY, currentTextSurface->w * scaleFactorForText, currentTextSurface-> h * scaleFactorForText};
        checkForError(SDL_RenderCopy(renderer, currentTextTexture, nullptr, &currentRect), error);
        currentY += scaleFactorForText * currentTextSurface->h;
        SDL_DestroyTexture(currentTextTexture);
    }

    //Clean up
    for(SDL_Surface* surface: textSurfaces){
        SDL_FreeSurface(surface);
    }
    if(error != ErrorCode::OK)
        std::cerr<<error<<", SDL_Error: "<<SDL_GetError()<< ", buttonText = "<<m_buttonData().text<<", rect = "<<m_buttonData().shapeAndPlace<<std::endl;
    return error;
}

SDL_Rect ButtonElement::m_areaRect(){
    return SDL_Rect{m_x, m_y, m_width, m_height};
}

bool ButtonElement::m_isHitBy(int x, int y){
    SDL_Rect area = m_areaRect();
    return Graphics::isWithinRect(x, y, area);
}
