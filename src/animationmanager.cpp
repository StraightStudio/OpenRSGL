#include <include/animationmanager.h>

AnimationManager::AnimationManager()
{

}

void AnimationManager::loadAnimations(AppConfig &conf)
{
    a_animations.clear();
    for( auto anim : conf.app_animations )
    {
        a_animations[anim.first.c_str()] = anim.second;

        Logger::log("AnimationManager", "Added new animation: "+anim.first+", "+QString::number(anim.second.frameCount).toStdString()+" frames.");
    }
}

const SDL_Rect &AnimationManager::frame(unistring anim, int i) const
{
    return a_animations[anim.c_str()].frames[i];
}
