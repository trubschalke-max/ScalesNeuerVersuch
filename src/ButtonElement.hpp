#ifndef BUTTONELEMENT_H
#define BUTTONELEMENT_H

#pragma once

#include "GameObject.hpp"
#include "Graphics.hpp"
#include<string>

class ButtonElement: public GameObject 
{
    public: struct ButtonData{
        SDL_Rect shapeAndPlace;
        std::string text;
    };
    enum Activeness{
        ACTIVE, HOVERED_OVER_AND_ACTIVE, PASSIVE, HOVERED_OVER_AND_PASSIVE, FORBIDDEN, PERMANENTLY_SELECTED, ACTIVENESS_ERROR
    };
    protected:
    Activeness m_activeness; //Active could also be named selected.
    int m_width;
    int m_height;
    std::string m_textDisplayed;

    static SDL_Color s_errorColor(){return Graphics::errorColor();}
public:
    ButtonElement(int x, int y, int width, int height, std::string text, GameObject::Layer layer = DEFAULT_LAYER, Activeness activeness = PASSIVE);
    ButtonElement(SDL_Rect shapeAndPlace, std::string text, GameObject::Layer layer = DEFAULT_LAYER, Activeness activeness = PASSIVE);
    ~ButtonElement();
    virtual ErrorCode m_draw(SDL_Renderer* renderer) override;
    virtual SDL_Color m_currentBackgroundColor() = 0;
    virtual SDL_Color m_currentTextColor() = 0;
    SDL_Rect m_areaRect();
    void m_setActiveness(Activeness a){m_activeness = a;}
    Activeness m_Activeness(){return m_activeness;}

    virtual bool m_isHitBy(int x, int y) override;

    ButtonData m_buttonData(){return ButtonData{SDL_Rect{m_x, m_y, m_width, m_height}, m_textDisplayed};}
    void m_setText(std::string newText){m_textDisplayed = newText;}
};


#endif