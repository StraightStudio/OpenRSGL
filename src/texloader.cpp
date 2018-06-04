#include "../include/texloader.h"

TexLoader::TexLoader()
{

}

TexLoader::~TexLoader()
{
    QList<QString> toDelete = texLib.keys();
    for(int i=0; i < toDelete.size(); i++)
    {
        SDL_DestroyTexture( texLib[toDelete.at(i)] );
        texLib[toDelete.at(i)] = 0;
    }
}

SDL_Texture *TexLoader::getTex(QString texAlias)
{
    return texLib[texAlias];
}

void TexLoader::addTex(QString file, QString texAlias, SDL_Renderer* rend)
{
    if(!IMG_Load( QString(IMG_ROOT+file).toStdString().c_str() ) ){
        Logger::err("TexLoader", "No such texture: " IMG_ROOT+file);
        return;
    }
    texLib[texAlias] = SDL_CreateTextureFromSurface(rend, IMG_Load( QString(IMG_ROOT+file).toStdString().c_str() ));
    Logger::log("Core", "'" IMG_ROOT+file+"' texture imported as '"+texAlias+"'.");
}

void TexLoader::loadTextures(SDL_Renderer* rend, AppConfig &conf)
{
    for(QString talias : conf.app_textures.keys())
    {
        addTex(conf.app_textures[talias], talias, rend);
    }
}
