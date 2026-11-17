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
    protected:
    ObjectData::Activeness m_activeness; //Active could also be named selected.
    int m_width;
    int m_height;
    std::string m_textDisplayed;

    static SDL_Color s_errorColor(){return Graphics::errorColor();}
public:
    ButtonElement(ObjectData::PlayerID playerID, int x, int y, int width, int height, std::string text, GameObject::Layer layer = DEFAULT_LAYER, ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE);
    ButtonElement(ObjectData::PlayerID playerID, SDL_Rect shapeAndPlace, std::string text, GameObject::Layer layer = DEFAULT_LAYER, ObjectData::Activeness activeness = ObjectData::Activeness::PASSIVE);
    ButtonElement(ButtonElement&&) = default;
    virtual ~ButtonElement();
    virtual ErrorCode m_draw(SDL_Renderer* renderer) override;
    virtual SDL_Color m_currentBackgroundColor() = 0;
    virtual SDL_Color m_currentTextColor() = 0;
    SDL_Rect m_areaRect();
    void m_setActiveness(ObjectData::Activeness a){
        m_activeness = a;
    }
    ObjectData::Activeness m_Activeness(){return m_activeness;}

    virtual bool m_isHitBy(int x, int y) override;

    ButtonData m_buttonData(){return ButtonData{SDL_Rect{m_x, m_y, m_width, m_height}, m_textDisplayed};}
    void m_setText(std::string newText){m_textDisplayed = newText;}
};


#endif