/*
 * Window.cpp
 *
 *  Created on: 15.02.2022
 *      Author: uwe-w
 */

#include <SDL2\SDL.h>
#include <iostream>
#include "Window.hpp"
#include<vector>

using namespace std;

Window* theWindow = nullptr;

Window::Window(std::string name, unsigned int width, unsigned int height)
:m_name(name), m_width(width), m_height(height)
{

}

ErrorCode Window::m_init()
{
	m_window = SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_UNDEFINED
			  , SDL_WINDOWPOS_UNDEFINED
			  , m_width, m_height, SDL_WINDOW_SHOWN );
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if(m_window == nullptr){
		return ErrorCode::FAILURE;
	}
	m_drawing = std::make_unique<Drawing>(m_renderer);
	return ErrorCode::OK;
}

Window::~Window(){
	SDL_DestroyWindow(m_window);
	cout<<"Window: Destruktor"<<endl;
}

unsigned int Window::m_Width(){
	return m_width;
}

unsigned int Window::m_Height(){
	return m_height;
}

SDL_Renderer* Window::m_Renderer(){
	return m_renderer;
}




