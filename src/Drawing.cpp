#include "Drawing.hpp"
#include "GameObject.hpp"
#include "ErrorCode.hpp"
#include<algorithm>
#include <string>
#include <iostream>

Drawing::Drawing(SDL_Renderer* renderer):m_renderer(renderer){}

ErrorCode Drawing::m_add(GameObject* newObject){
    auto it = std::find(m_queue.begin(), m_queue.end(), newObject);
    if(it != m_queue.end()){
        return ErrorCode::DUPLICATE_LIST_INSERTION;
    }
    it = std::find_if(m_queue.begin(), m_queue.end(), [newObject](GameObject* current){return current->m_Layer()>newObject->m_Layer();});
    m_queue.insert(it, newObject);
    return ErrorCode::OK; 
}

ErrorCode Drawing::m_draw(){
    ErrorCode whatToReturn;
    for(GameObject* objectToDraw: m_queue){
        whatToReturn = objectToDraw->m_draw(m_renderer);
    }
    return whatToReturn;
}
