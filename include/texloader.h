#ifndef TEXLOADER_H
#define TEXLOADER_H

#include <include/depends.h>
#include <include/logger.h>

class TexLoader
{
public:
    TexLoader();
    ~TexLoader();
    SDL_Texture *getTex(QString texAlias);
    void addTex(QString file, QString texAlias, SDL_Renderer *rend);

    void loadTextures(SDL_Renderer *rend, AppConfig &conf);
private:
    QMap <QString, SDL_Texture*> texLib;
};

#endif // TEXLOADER_H
