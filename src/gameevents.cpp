#include "../include/gameevents.h"

vec2 GameEvents::mousePos()
{
    int x,y;
    SDL_GetMouseState(&x, &y);
    return vec2(x, y);
}

bool GameEvents::isMouseOver(SDL_Rect *rect)
{
    int x,y;
    SDL_GetMouseState(&x, &y);
    if(x <= rect->x+rect->w && x >= rect->x &&
       y <= rect->y+rect->h && y >= rect->y)
        return true;
    else
        return false;
}

bool GameEvents::isMouseDown(int mbtn)
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mbtn); // 1 - left, 2 - middle, 3 - right
}

bool GameEvents::isMouseUp(int mbtn)
{
    return !( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mbtn) ); // 1 - left, 2 - middle, 3 - right
}

GameEvents::GameEvents()
{

}
