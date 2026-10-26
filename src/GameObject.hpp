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
    enum{POSITION_UNDEFINED = -2000};
    enum Longevity{PERMANENT, ONE_ROUND_LONG};
protected: 
    Longevity m_longevity = ONE_ROUND_LONG;
    int m_x = POSITION_UNDEFINED;
    int m_y = POSITION_UNDEFINED;
    Layer m_layer;
    std::unique_ptr<Suggestor> m_suggestor;
    std::unique_ptr<DisplayingSupporter> m_displayingSupporter;
    HandlingMode m_mode = SUGGEST;
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
    virtual ErrorCode m_putIntoEffect(const Suggestor::Suggestion& suggestion); //The default behavior is to throw an exception because this should not have been called in the default case. It is expected from any implementation that display upgrades are being triggered by this function.
    virtual ErrorCode m_resetSuggestion();
    const Suggestor::Suggestion& m_CurrentSuggestion(){return m_currentSuggestion;}
    virtual void m_setPosition(int x, int y){m_x = x; m_y = y;} //When created by the factory, the initial position is not yet clear. Thus, a setter is needed. The virtuality comes from the fact that certain objects consist of many subobjects.
    virtual bool m_isRelevant(const Suggestor::Suggestion& suggestion){return false;}//A, say, explainablesetelement should not deal with a scales related suggestion.
    static std::initializer_list<int> s_sdlEventTypesRelevantForGameObjects();
    void m_setHandlingMode(HandlingMode mode){m_mode = mode;}
    auto m_HandlingMode(){return m_mode;}
    ErrorCode m_updateDisplaying(); //This redirects to the displaying supporter's m_updateDisplaying().
    Longevity m_Longevity(){return m_longevity;}
    void m_setLongevity(Longevity longevity){m_longevity = longevity;}

    friend class Player;

};

#endif