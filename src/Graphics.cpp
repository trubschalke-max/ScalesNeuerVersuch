#include "Graphics.hpp"
SDL_Color Graphics::whiteColor(){return SDL_Color{250, 250, 250};}
SDL_Color Graphics::blackColor(){return SDL_Color{1,1,1};}
SDL_Color Graphics::brownColor(){return SDL_Color{130,80,0};}
SDL_Color Graphics::infoTextColor(){return Graphics::blackColor();}
SDL_Color Graphics::infoTextBackgroundColor(){return SDL_Color{125,125,140};}
SDL_Color Graphics::darkgreyColor(){return SDL_Color{72,72,72};}
SDL_Color Graphics::shieldGreyColor(){return SDL_Color{138,138,142};}
SDL_Color Graphics::resourcesWhiteColor(){return SDL_Color{235,235,235};}
SDL_Color Graphics::standardSelectorMarkedColor(){return SDL_Color{50,50,255};}
SDL_Color Graphics::greenColor(){return SDL_Color{20,200,20};}
SDL_Color Graphics::yellowColor(){
	return SDL_Color{255,255,150};
}
SDL_Color Graphics::turquoiseColor(){
	return SDL_Color{12,227,235};
}
TTF_Font* Graphics::theFont;