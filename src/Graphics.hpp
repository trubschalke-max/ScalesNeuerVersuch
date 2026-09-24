#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include<stdexcept>
namespace Graphics{
class Font{
private:
    TTF_Font* m_font;
public:
    Font(){throw std::runtime_error("Fonts not supported.");}
    TTF_Font* m_get(){return m_font;}
    ~Font(){TTF_CloseFont(m_font);}
};
SDL_Color darkgreyColor();
SDL_Color shieldGreyColor();
SDL_Color resourcesWhiteColor();
SDL_Color standardSelectorMarkedColor();
SDL_Color greenColor();
SDL_Color yellowColor();
SDL_Color turquoiseColor();
SDL_Color whiteColor();
SDL_Color blackColor();
SDL_Color brownColor();
SDL_Color infoTextColor();
SDL_Color infoTextBackgroundColor();
extern TTF_Font* theFont;
}