/*
 * Window.hpp
 *
 *  Created on: 15.02.2022
 *      Author: uwe-w
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SDL2\SDL.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "ErrorCode.hpp"

class WindowInitFail: public std::exception{
	 std::string what(){return "Some window could not be created.";}
};

class Window{
private:
	std::string m_name;
	SDL_Renderer* m_renderer;
	SDL_Window* m_window;
	//werd ich gucken m�ssen, ob list oder vector
	//Idee: mehrere Drawings pro Window, dass man switchen kann.
	unsigned int m_width;
	unsigned int m_height;
public:
enum Lengths{SCREEN_WIDTH = 1200, SCREEN_HEIGHT = 720};
	~Window();
Window(std::string name, unsigned int width = SCREEN_WIDTH, unsigned int height = SCREEN_HEIGHT);
	unsigned int m_Height();
	unsigned int m_Width();
	SDL_Renderer* m_Renderer();
	ErrorCode m_Init();
	std::string m_Name();
	SDL_Window* m_Window(){return m_window;}
};

extern Window* theWindow;

#endif /* WINDOW_HPP_ */
