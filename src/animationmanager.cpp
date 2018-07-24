#include <animationmanager.h>

AnimationManager::AnimationManager()
{

}

void AnimationManager::clear()
{
    a_animations.clear();
}

void AnimationManager::loadAnimations(AppConfig &conf)
{
    a_animations.clear();
    for( auto anim : conf.app_animations )
    {
        a_animations[anim.first.c_str()] = anim.second;

        Logger::log("AnimationManager", "Added new animation: "+anim.first+", "+to_string(anim.second.frameCount)+" frames.");
    }
}

const SDL_Rect &AnimationManager::frame(unistring anim, int i) const
{
    return a_animations.at(anim).frames.at(i);
}
