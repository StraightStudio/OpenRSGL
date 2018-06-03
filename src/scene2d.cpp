#include "../include/scene2d.h"

Scene2d::Scene2d() :
    sinfo("", "")
{

}

void Scene2d::spawn(vec2 pos, vec2 dim, QString textureAlias, TexLoader *tl)
{
    Actor2d actor;
    actor.setPos(pos);
    actor.setDim(dim);

    m_objs.push_back(actor);
}

void Scene2d::spawn(vec2 pos, QString textureAlias, TexLoader *tl)
{
    Actor2d actor;
    actor.setPos(pos);
    actor.setDim(128, 128);

    m_objs.push_back(actor);
}

void Scene2d::addActor(vec2 pos, vec2 dim, QStringList textures)
{
    Actor2d actor;

    actor.setPos(pos);
    actor.setDim(dim);
    actor.setTexs(&textures);

    m_objs.push_back(actor);
}

SDL_Rect &Scene2d::getRect(int id)
{
    return m_objs.at(id).rect;
}

vector<Actor2d> &Scene2d::objs()
{
    return m_objs;
}
