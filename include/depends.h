#ifndef DEPENDS_H
#define DEPENDS_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include <QString>
#include <QMap>

#include <include/config.h>

using namespace std;

struct vec2
{
    int x,y;
    vec2(int a, int b):
        x(a), y(b){}

};

#endif // DEPENDS_H
