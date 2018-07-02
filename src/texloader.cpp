#include <include/texloader.h>

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

SDL_Texture *TexLoader::getTex(unistring texAlias)
{
    return texLib[texAlias.c_str()];
}

void TexLoader::addTex(unistring file, unistring texAlias, SDL_Renderer* rend)
{
    if(!IMG_Load( unistring(IMG_ROOT+file).c_str() ) ){
        Logger::err("TexLoader", "No such texture: " IMG_ROOT+file);
        return;
    }
    texLib[texAlias.c_str()] = SDL_CreateTextureFromSurface(rend, IMG_Load( unistring(IMG_ROOT+file).c_str() ));
    Logger::log("TexLoader", "'" IMG_ROOT+file+"' texture imported as '"+texAlias+"'.");
}

void TexLoader::loadTextures(SDL_Renderer* rend, AppConfig &conf)
{
    for(auto texture : conf.app_textures)
    {
        Logger::log("TexLoader", "Loading '"+texture.first+"'...");
        addTex(texture.second, texture.first, rend);
    }
}
