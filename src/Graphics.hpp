#ifndef GAME_HPP
#define GAME_HPP
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "myExceptions.hpp"
#include "ErrorCode.hpp"
#include<iostream>
#include<memory>

namespace Graphics{
    class Font{
private:
    TTF_Font* m_font;
    int m_fontSize;
public:
    enum FONT_SIZES{STANDARD_FONT_SIZE = 12};
    Font(std::string filename, int fontSize = STANDARD_FONT_SIZE);
    TTF_Font* m_get(){return m_font;}
    ~Font(){std::cout<<"Font-Destruktor"<<std::endl; TTF_CloseFont(m_font);}
};
SDL_Color darkgreyColor();
SDL_Color shieldGreyColor();
SDL_Color resourcesWhiteColor();
SDL_Color standardSelectorMarkedColor();
SDL_Color greenColor();
SDL_Color yellowColor();
SDL_Color turquoiseColor();
SDL_Color whiteColor();
SDL_Color blackColor();
SDL_Color brownColor();
SDL_Color infoTextColor();
SDL_Color infoTextBackgroundColor();
extern Font* theFont;
SDL_Rect displayText(const std::string& str, int x, int y, SDL_Color color, SDL_Renderer* renderer, bool shaded, SDL_Color backgroundColor);
SDL_Rect printMultipleLines(const std::string& str, int x, int y, SDL_Color color, SDL_Renderer* renderer, bool shaded = false, SDL_Color backgroundColor = SDL_Color{0, 0, 0});
SDL_Rect printMultipleLines(const std::string& str, int x, int y, SDL_Color color, SDL_Renderer* renderer, SDL_Color backgroundColor);
}
std::ostream& operator<<(std::ostream& os, const SDL_Rect& rect);

#endif