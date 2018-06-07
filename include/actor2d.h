#ifndef ACTOR2D_H
#define ACTOR2D_H

#include <include/depends.h>

class Actor2d
{
public:
    Actor2d();
    SDL_Rect rect;
    QString curAnim;

    QMap <QString, Animation2d> animations; // idle, walk, attack, die, etc...
    int curFrame; int frameIter;
    QString name;
    QString type;
    QMap<QString,QString> trigger;

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

    const SDL_Rect &getFrame() const
    {
        return animations[curAnim].getFrame(curFrame); // Get frame from animation.
    }

    void setName(QVariant n)
    {
        name = n.toString();
    }

    const QString &getName() const
    {
        return name;
    }

    SDL_Rect &getRect()
    {
        return rect;
    }

    int triggerAction(QString event)
    {
        if(trigger.contains(event))
        {
            if(trigger[event].split(" ")[0] == "snd")
                return SOUND_ACTION;
            else if(trigger[event].split(" ")[0] == "scn")
                return SCENE_ACTION;
            else if(trigger[event].split(" ")[0] == "quit")
                return QUIT_ACTION;
        }
        else
            return 0;
    }
    QString triggerArgument(QString event)
    {
        if(trigger.contains(event))
            return trigger[event].split(" ")[1];
    }

    void setType(QVariant t)
    {
        type = t.toString();
    }

    void setAnim(QString alias, Animation2d anim)
    {
        animations[alias] = anim;
    }

    void nextFrame()
    {
        if(animations[curAnim].fps > 0)
        {
            if(frameIter == TARGET_FPS/animations[curAnim].fps) // TARGET_FPS/frameRate MUST BE INT
            {
                frameIter=0;
                if(curFrame == animations[curAnim].frameCount-1)
                    curFrame=0;
                else
                    curFrame++;
            }
            else
                frameIter++;
        }
    }

    void setTexs(QStringList texs, QList<SDL_Rect> frames);
};

#endif // ACTOR2D_H
