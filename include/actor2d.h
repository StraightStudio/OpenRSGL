#ifndef ACTOR2D_H
#define ACTOR2D_H

#include <include/depends.h>

class Actor2d
{
public:
    Actor2d();
    SDL_Rect rect;
    QStringList texs;


    void setPos(vec2 pos)
    {
        rect.x = pos.x;
        rect.y = pos.y;
    }
    void setPos(int x, int y)
    {
        rect.x = x;
        rect.y = y;
    }
    const vec2 &getPos() const
    {
        return vec2(rect.x, rect.y);
    }

    void setDim(vec2 dim)
    {
        rect.w = dim.x;
        rect.h = dim.y;
    }
    void setDim(int w, int h)
    {
        rect.w = w;
        rect.h = h;
    }
    const vec2 &getDim() const
    {
        return vec2(rect.w, rect.h);
    }

    const QString &getTex(int id) const
    {
        return texs.at(id);
    }
    void setTexs(QStringList *texlist);
};

#endif // ACTOR2D_H
