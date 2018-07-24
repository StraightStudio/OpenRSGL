#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <depends.h>
#include <config.h>

class AnimationManager
{
public:
    AnimationManager();
    void clear();

    void loadAnimations(AppConfig &conf);

    const SDL_Rect &frame(unistring anim, int i) const; // i - id of frame.

    int fcount(unistring anim)
    {
        return a_animations[anim].frameCount;
    }
    int fps(unistring anim)
    {
        return a_animations[anim].fps;
    }
private:
    map<unistring, Animation2d> a_animations;
};

#endif // ANIMATIONMANAGER_H
