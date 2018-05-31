#include "../include/scene2d.h"

Scene2d::Scene2d()
{

}

void Scene2d::draw(SDL_Renderer *rend)
{
    for(size_t i=0; i < m_objs.size(); i++)
    {
        SDL_RenderCopy(rend, m_objs.at(i).getTex(0), NULL, &m_objs.at(i).rect);
    }
}

void Scene2d::spawn(vec2 pos, vec2 dim, QString textureAlias, TexLoader *tl)
{
    Actor2d actor;
    actor.setPos(pos);
    actor.setDim(dim);
    actor.addTex(tl->getTex(textureAlias)); // Idle texture pos (0)

    m_objs.push_back(actor);
}

void Scene2d::spawn(vec2 pos, QString textureAlias, TexLoader *tl)
{
    Actor2d actor;
    actor.setPos(pos);
    actor.addTex(tl->getTex(textureAlias)); // Idle texture pos (0)
    actor.setDim(128, 128);

    m_objs.push_back(actor);
}
