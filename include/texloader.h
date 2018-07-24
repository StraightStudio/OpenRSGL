#ifndef TEXLOADER_H
#define TEXLOADER_H

#include <depends.h>
#include <config.h>
#include <logger.h>

class TexLoader
{
public:
    TexLoader();
    SDL_Texture *getTex(unistring texAlias);
    void addTex(unistring file, unistring texAlias, SDL_Renderer *rend);

    void clear();

    void loadTextures(SDL_Renderer *rend, AppConfig &conf);
    map<unistring, SDL_Texture*> texLib;

protected:
    SDL_Surface* tmpsurf;
};

#endif // TEXLOADER_H
