#include "../include/scene2d.h"

Scene2d::Scene2d() :
    sinfo("", "")
{

}

void Scene2d::start(AudioManager *mgr)
{
    mgr->playMusic(sinfo.bg_track, true);
}

void Scene2d::addActor(AppConfig &conf, vec2 pos, vec2 dim, QString name, QString type, QMap<QString, QString> trigger, QString anim)
{
    Actor2d actor;

    actor.setPos(pos);
    actor.setDim(dim);
    actor.setName(name);
    actor.setType(type);

    actor.trigger = trigger;
    actor.curAnim = anim;
    actor.setAnim(anim, conf.app_animations[anim]);

    m_objs[name] = actor;
}

SDL_Rect &Scene2d::getRect(QString name)
{
    return m_objs[name].rect;
}

QMap<QString, Actor2d> &Scene2d::objs()
{
    return m_objs;
}
