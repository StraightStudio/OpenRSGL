#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <include/depends.h>

class AnimationManager
{
public:
    AnimationManager();
    void loadAnimations(AppConfig &conf);

    const SDL_Rect &frame(QString anim, int i) const; // i - id of frame.

    int fcount(QString anim)
    {
        return a_animations[anim].frameCount;
    }
    int fps(QString anim)
    {
        return a_animations[anim].fps;
    }

private:
    QMap <QString, Animation2d> a_animations;
};

#endif // ANIMATIONMANAGER_H
