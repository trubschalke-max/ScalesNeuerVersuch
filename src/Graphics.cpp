#include "Graphics.hpp"
#include "myExceptions.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include<vector>
#include<iostream>
using namespace std;

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
SDL_Color Graphics::errorColor(){return SDL_Color{140, 140, 30};}
Graphics::Font* Graphics::theFont;
Graphics::Font::Font(std::string filename, int fontSize):m_fontSize(fontSize)
{m_font = TTF_OpenFont(filename.c_str(), m_fontSize);}

//From here, it's legacy code

int SDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, (unsigned char) color.r,(unsigned char)color.g,(unsigned char)color.b,(unsigned char)color.a);
}

std::vector<SDL_Surface*> Graphics::createSurfacesFromString(const std::string& str, SDL_Color color, SDL_Renderer* renderer, SDL_Rect* overallRect){
	int maxWidth = 0;
	int height = 0;
	std::string stringOfInterest = str;
	if(stringOfInterest.back()!='\n')
		stringOfInterest+="\n";
	std::string temporaryString = "";
	int length = stringOfInterest.length();
	std::vector<SDL_Surface*> textures;
	for(int i(0); i<length; i++){
		if(stringOfInterest[i] != '\n'){
			temporaryString += stringOfInterest[i];
			continue;
		}
		else{
			SDL_Surface* surface = TTF_RenderText_Blended(Graphics::theFont->m_get(), temporaryString.c_str(), color);
			//	std::cout<<temporaryString<<std::endl;
			temporaryString = "";
			if(!surface){
				continue;
			}
			maxWidth=std::max(maxWidth,surface->w);
			height+=surface->h;
			textures.push_back(surface);
		}
	}
	if(overallRect){
		overallRect->x = 0;
		overallRect->y = 0;
		overallRect->w = maxWidth;
		overallRect->h = height;
	}
	return textures;
}


SDL_Rect Graphics::printMultipleLines(const std::string& str, int x, int y, SDL_Color color, SDL_Renderer* renderer,  bool shaded, SDL_Color backgroundColor){
	int reachedHeight = 0;
	SDL_Rect rectToFill{x,y};
	std::vector<SDL_Surface*> textures = Graphics::createSurfacesFromString(str, color, renderer, &rectToFill);
	if(shaded){
		if(FillRect(rectToFill, color, renderer) != ErrorCode::OK)
			throw std::runtime_error("Fail while printing shadedly!");
	}
	for(SDL_Surface* currentSurface: textures){
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, currentSurface);
		SDL_Rect textRect{x, reachedHeight+y, currentSurface->w, currentSurface->h};
		reachedHeight+=currentSurface->h;
		SDL_RenderCopy(renderer, texture, nullptr,&textRect);
		SDL_FreeSurface(currentSurface);
		SDL_DestroyTexture(texture);
	}
	return rectToFill;
}


SDL_Rect Graphics::displayText(const std::string& str, int x, int y, SDL_Color color, SDL_Renderer* renderer, bool shaded, SDL_Color backgroundColor){
	SDL_Rect whatToReturn = printMultipleLines(str,x,y,color,renderer,shaded,backgroundColor);
	SDL_Event e;
	bool quitSurface = false;
	while(!quitSurface){
		try{
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				throw SDLQuitException();
			}
			if(e.type==SDL_KEYDOWN || e.type==SDL_MOUSEBUTTONDOWN){
				quitSurface = true;
			}
		}
		}
		catch(QuitSurface &qs){
			quitSurface = true;
		}
		catch(SDLQuitException& sdlqe){
			throw sdlqe;
		}
	}
	return whatToReturn;
}

SDL_Rect Graphics::printMultipleLines(const std::string& str, int x, int y, SDL_Color color, SDL_Renderer* renderer, SDL_Color backgroundColor){
	return Graphics::printMultipleLines(str, x, y, color, renderer, true, backgroundColor);
}

ErrorCode Graphics::FillRect(SDL_Rect rectToFill, SDL_Color color, SDL_Renderer* renderer){
	ErrorCode error = ErrorCode::OK;
	SDL_Color oldRenderDrawColor;
	checkForError(SDL_GetRenderDrawColor(renderer,&(oldRenderDrawColor.r),&(oldRenderDrawColor.g),&(oldRenderDrawColor.b),&(oldRenderDrawColor.a)), error);
	checkForError(SDL_SetRenderDrawColor(renderer, color), error);
	checkForError(SDL_RenderFillRect(renderer, &rectToFill), error);
	checkForError(SDL_SetRenderDrawColor(renderer, oldRenderDrawColor), error);
	return error;
}

bool Graphics::isWithinRect(int x, int y, SDL_Rect& rect){
	 // Too far left
    if (x < rect.x) return false;
    // Too far right
    if (x >= rect.x + rect.w) return false;
    // Too far up
    if (y < rect.y) return false;
    // Too far down
    if (y >= rect.y + rect.h) return false;

	return true;
}

std::ostream& operator<<(std::ostream& os, const SDL_Rect& rect){
	return os<<"SDL_Rect{x = "<<rect.x<<", y = "<<rect.y<<", w = "<<rect.w<<", h = "<<rect.h<<"}";
}
