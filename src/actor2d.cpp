#include "../include/actor2d.h"

Actor2d::Actor2d()
{
    frameIter   = 0;
    frameRate   = 0;
    curTex      = 0;
}

void Actor2d::setTexs(QStringList* texlist)
{
    for(int i=0; i < texlist->count(); i++)
        texs.append(texlist->at(i));
}
