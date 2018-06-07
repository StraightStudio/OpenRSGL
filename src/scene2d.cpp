#include "../include/scene2d.h"

Scene2d::Scene2d() :
    sinfo("", "")
{

}

void Scene2d::start(AudioManager *mgr)
{
    mgr->playMusic(sinfo.bg_track, true);
}

void Scene2d::addActor(vec2 pos, vec2 dim, QString name, QString type, QMap<QString, QString> trigger, QString anim)
{
    Actor2d actor;

    actor.setPos(pos);
    actor.setDim(dim);
    actor.setName(name);
    actor.setType(type);

    actor.trigger = trigger;
    actor.curAnim = anim;

    m_objs[name] = actor;
}

void Scene2d::loadAnimations(AppConfig &conf)
{
    for(QString alias : m_objs.keys())
    {
        m_objs[alias].setTexs( conf.app_animations.keys(), conf.app_animations[m_objs[alias].curAnim] );
        m_objs[alias].setFps(conf.anim_fps[m_objs[alias].curAnim]);
        Logger::log("Scene2d", "Set fps to "+QString::number(m_objs[alias].frameRate));
    }
}

SDL_Rect &Scene2d::getRect(QString name)
{
    return m_objs[name].rect;
}

QMap<QString, Actor2d> &Scene2d::objs()
{
    return m_objs;
}
