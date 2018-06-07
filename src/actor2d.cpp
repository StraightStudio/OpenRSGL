#include "../include/actor2d.h"

Actor2d::Actor2d()
{
    frameIter   = 0;
    curFrame      = 0;
    curAnim     = "idle";
}

void Actor2d::setTexs(QStringList texs, QList<SDL_Rect> frames)
{
    animations.clear();
    for(QString tex : texs)
    {
        for(SDL_Rect r : frames)
            animations[tex].addFrame(r);
    }
}
