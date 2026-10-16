#ifndef GAME_OBJECT
#define GAME_OBJECT

#include<SDL2/SDL.h>
#include "ErrorCode.hpp"
#include "SuggestSupport/Suggestor.hpp"
#include "SuggestSupport/DisplayingSupporter.hpp"

class GameObject{
public:
    enum Layer{DEFAULT_LAYER = 0};
    enum HandlingMode{DO_NOTHING, DISPLAY_ONLY, SUGGEST, DIRECT_HANDLING};
    enum Relevance{IRRELEVANT, RELEVANT_FOR_DISPLAYING, RELEVANT = DIRECT_HANDLING};
protected: 
    int m_x;
    int m_y;
    Layer m_layer;
    std::unique_ptr<Suggestor> m_suggestor;
    std::unique_ptr<DisplayingSupporter> m_displayingSupporter;
    HandlingMode m_mode = DIRECT_HANDLING;
    Suggestor::Suggestion m_currentSuggestion;
    ObjectData::PlayerID m_playerID = ObjectData::PlayerID::PLAYER_NONE;
public:
    GameObject(ObjectData::PlayerID playerID, int x, int y, Layer layer = DEFAULT_LAYER);
    GameObject(GameObject&&) = default;
    virtual ~GameObject();
    virtual ErrorCode m_draw(SDL_Renderer* renderer) = 0;
    virtual bool m_isHitBy(int x, int y) = 0;
    ErrorCode m_handleEvent(const SDL_Event& e); //This redirects to m_changeDisplayingDueTo() or m_suggestFor(), possibly combined with m_putIntoEffect(), based on m_relevanceOf(). On direct handling, the suggestion made will be reset. In the future, I might save it in some kind of logging system.
    int m_X(){return m_x;}
    int m_Y(){return m_y;}
    int m_Layer(){return m_layer;}
    Suggestor::Suggestion m_suggestFor(const SDL_Event& event); //This redirects to the suggestor's m_suggestFor(), and puts the playerID into the suggestion.
    ErrorCode m_changeDisplayingDueTo(const SDL_Event& event); //The expected results are either NOTHING_HAPPENED or MERELY_VISUAL_IMPLICATIONS.
    Relevance m_relevanceOf(const SDL_Event& event);
    virtual ErrorCode m_putIntoEffect(const Suggestor::Suggestion& suggestion); //The default behavior is to throw an exception because this should not have been called in the default case.
    virtual ErrorCode m_resetSuggestion();
    const Suggestor::Suggestion& m_CurrentSuggestion(){return m_currentSuggestion;}
    void m_setPosition(int x, int y){m_x = x; m_y = y;} //When created by the factory, the initial position is not yet clear. Thus, a setter is needed.

    static std::initializer_list<int> s_sdlEventTypesRelevantForGameObjects();

    friend class Player;

};

#endif