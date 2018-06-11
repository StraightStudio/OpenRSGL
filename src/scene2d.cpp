#include "../include/scene2d.h"

Scene2d::Scene2d() :
    sinfo("", "")
{

}

void Scene2d::start(AudioManager *mgr)
{
    mgr->playMusic(sinfo.bg_track, true);
}

void Scene2d::addActor(vec2 pos, vec2 dim, vec2 rdim, QString name, QString type, QMap<QString, QString> trigger, QString texture, QString anim_idle)
{
    Actor2d actor;

    actor.setPos(pos);
    actor.setDim(dim);
    actor.setRealDim(rdim);
    actor.setName(name);
    actor.setType(type);
    actor.setTex(texture);

    actor.trigger = trigger;
    actor.idle_anim = anim_idle;
    actor.curAnim = actor.idle_anim;

    m_objs[name] = actor;
}


void Scene2d::addActor(vec2 pos, vec2 dim, vec2 rdim, QString name, QString type, QString texture, QString anim_idle)
{
    Actor2d actor;

    actor.setPos(pos);
    actor.setDim(dim);
    actor.setRealDim(rdim);
    actor.setName(name);
    actor.setType(type);
    actor.setTex(texture);

    actor.idle_anim = anim_idle;
    actor.curAnim = actor.idle_anim;

    if(type == "button" || type == "actor")
        m_objs.insert(m_objs.constEnd() , name, actor);
    else if(type == "building")
        m_objs.insert(m_objs.constBegin() , name, actor);
}

SDL_Rect &Scene2d::getRect(QString name)
{
    return m_objs[name].rect;
}

QMap<QString, Actor2d> &Scene2d::objs()
{
    return m_objs;
}
