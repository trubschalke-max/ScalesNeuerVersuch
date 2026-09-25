#ifndef BUTTONELEMENT_H
#define BUTTONELEMENT_H

#pragma once

#include "GameObject.hpp"
#include<string>

class ButtonElement: public GameObject 
{
    public: struct ButtonData{
        SDL_Rect shapeAndPlace;
        std::string text;
    };
protected:
    bool m_isActive;
    int m_width;
    int m_height;
    std::string m_textDisplayed;
public:
    ButtonElement(int x, int y, int width, int height, std::string text, GameObject::Layer layer = DEFAULT_LAYER, bool isActive = true);
    ButtonElement(SDL_Rect shapeAndPlace, std::string text, GameObject::Layer layer = DEFAULT_LAYER, bool isActive = true);
    ~ButtonElement();
    virtual ErrorCode m_draw(SDL_Renderer* renderer) override;
    virtual SDL_Color m_currentBackgroundColor() = 0;
    virtual SDL_Color m_currentTextColor() = 0;

    ButtonData m_buttonData(){return ButtonData{SDL_Rect{m_x, m_y, m_width, m_height}, m_textDisplayed};}
    void m_setText(std::string newText){m_textDisplayed = newText;}
};

#endif