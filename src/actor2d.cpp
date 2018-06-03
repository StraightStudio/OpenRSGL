#include "../include/actor2d.h"

Actor2d::Actor2d()
{

}

void Actor2d::setTexs(QStringList* texlist)
{
    for(int i=0; i < texlist->count(); i++)
        texs.append(texlist->at(i));
}
