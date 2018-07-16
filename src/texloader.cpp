
#ifdef _WIN64
#include <texloader.h>
#else 
#include <include/texloader.h>
#endif

TexLoader::TexLoader()
{

}

SDL_Texture *TexLoader::getTex(unistring texAlias)
{
    return texLib[texAlias.c_str()];
}

void TexLoader::addTex(unistring file, unistring texAlias, SDL_Renderer* rend)
{
    tmpsurf = IMG_Load( unistring(IMG_ROOT+file).c_str() );
    if(tmpsurf == nullptr)
        Logger::err("TexLoader", "No such texture: " IMG_ROOT+file);

    texLib[texAlias] = SDL_CreateTextureFromSurface(rend, tmpsurf);
    Logger::log("TexLoader", "'" IMG_ROOT+file+"' texture imported as '"+texAlias+"'.");

    SDL_FreeSurface(tmpsurf);
}

void TexLoader::clear()
{
    for(auto &tid : texLib)
    {
        SDL_DestroyTexture( texLib[tid.first] );
        if(texLib[tid.first] != nullptr)
            texLib[tid.first] = nullptr;
    }
}

void TexLoader::loadTextures(SDL_Renderer* rend, AppConfig &conf)
{
    for(auto &texture : conf.app_textures)
    {
        Logger::log("TexLoader", "Loading '"+texture.first+"'...");
        addTex(texture.second, texture.first, rend);
    }
}
