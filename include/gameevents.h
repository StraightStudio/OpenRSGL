#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#include "../include/depends.h"
#include "../include/actor2d.h"

class GameEvents
{
public:
    static vec2 mousePos();
    static bool isMouseOver(SDL_Rect *rect);
    static bool isMouseDown(int mbtn);
    static bool isMouseUp(int mbtn);
private:
    GameEvents();
};

#endif // GAMEEVENTS_H
