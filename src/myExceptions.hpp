#ifndef MY_EXCEPTIONS
#define MY_EXCEPTIONS 

#include <exception>
#include <stdexcept>
#include<sstream>
#include "ErrorCode.hpp"

class NullPointerException: public std::exception{
public:
	std::string m_class;
	std::string m_functionData = "";
	NullPointerException(std::string s, std::string ss = ""): m_class(s), m_functionData(ss){}
	const char* what(){std::string whatToReturn; whatToReturn = "NullPointerException: "+m_class+"\n"+m_functionData; return whatToReturn.c_str();}
};

class InvalidCoordinate: public std::exception{
public: InvalidCoordinate(int x, int y):m_x(x), m_y(y){}
	int m_x;
    int m_y;
	const char* what(){std::stringstream s; s<<"Invalid coordinate: m_where = "<<m_x<<", "<<m_y<<std::endl; s.flush();return s.str().c_str();}
};

class LittleExceptionToDisplay: public std::runtime_error{
	using std::runtime_error::runtime_error;
};

class SDLQuitException: public std::exception{
public: ErrorCode m_error;
	SDLQuitException(ErrorCode error = ErrorCode::OK): m_error(error){}
	const char* what(){std::string s = "SDLQuitException" + toString(m_error); return s.c_str();}
};

class QuitSurface: public std::runtime_error{
public: QuitSurface():std::runtime_error("QuitSurface"){}
};

class UnsupportedSDLEvent: public std::exception{
public:
	SDL_Event m_event;
	UnsupportedSDLEvent(const SDL_Event& event): m_event(event){}
	const char* what() const noexcept{return ("Unsupported SDL event: type = " + std::to_string(m_event.type)).c_str(); }
};


#endif