#include "../include/animationmanager.h"

AnimationManager::AnimationManager()
{

}

void AnimationManager::loadAnimations(AppConfig &conf)
{
    a_animations.clear();
    for(QString animation_alias : conf.app_animations.keys())
    {
        a_animations[animation_alias] = conf.app_animations[animation_alias];

        Logger::log("AnimationManager", "Added new animation: "+animation_alias+", "+QString::number(conf.app_animations[animation_alias].frameCount)+" frames.");
    }
}

const SDL_Rect &AnimationManager::frame(QString anim, int i) const
{
    return a_animations[anim].frames[i];
}
