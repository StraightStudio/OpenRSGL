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
        Logger::err("TexLoader", "No such texture: "IMG_ROOT+file);
        return;
    }
    texLib[texAlias] = SDL_CreateTextureFromSurface(rend, IMG_Load( QString(IMG_ROOT+file).toStdString().c_str() ));
    Logger::log("Core", "'"IMG_ROOT+file+"' texture imported successfully.");
}

void TexLoader::loadTextures(SDL_Renderer* rend)
{
    QFile tlist(RES_ROOT"texture.list");
    tlist.open(QIODevice::ReadOnly);
    if(!tlist.isOpen()){
        Logger::err("Core", "Can't find 'data/texture.list'.");
        exit(-1);
    }
    QTextStream in(&tlist);

    QString line;
    QStringList parts;

    while(!in.atEnd())
    {
        line = in.readLine();
        parts = line.split(":");

        if(parts.count() > 2)
        {
            Logger::err("Core", "Incorrect structure format in 'data/texture.list'.");
            exit(-1);
        }
        addTex(parts.at(1).trimmed(), parts.at(0).trimmed(), rend);
    }
    tlist.close();
}
