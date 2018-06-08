#ifndef ACTOR2D_H
#define ACTOR2D_H

#include <include/depends.h>

enum DIRECTIONS {
    UP_DIR=0,
    DOWN_DIR=4,

    UP_LEFT_DIR=1,
    LEFT_DIR=2,
    DOWN_LEFT_DIR=3,

    UP_RIGHT_DIR=7,
    RIGHT_DIR=6,
    DOWN_RIGHT_DIR=5,

    IDLE=-1
};

class Actor2d
{
public:
    Actor2d();
    SDL_Rect rect;
    SDL_Rect real_rect; // Rect with 'real' width and height
    QString curAnim;
    QString texture;

    QStringList animations; // idle, walk, attack, die, etc...
    QString idle_anim;      // animation default name: soldier, tank, etc. ( will be soldier_up, soldier_down )

    int curFrame; int frameIter;
    QString name;
    QString type;
    QMap<QString,QString> trigger;



    // ========== G A M E   L O G I C   V A R I A B L E S ===========================
    vec2 targetPos; // Moving to...
    int move_speed;
    int move_direction;
    //
    int health_percentage;
    SDL_Rect health_rect;
    // ==============================================================================

    const QString &tex() const
    {
        return texture;
    }

    void setTex(QVariant t)
    {
        texture = t.toString();
    }

    void setPos(vec2 pos)
    {
        targetPos = pos;
        rect.x = pos.x;
        rect.y = pos.y;
        real_rect.x = pos.x;
        real_rect.y = pos.y;
    }
    void setPos(int x, int y)
    {
        targetPos = vec2(x, y);
        rect.x = x;
        rect.y = y;
        real_rect.x = x;
        real_rect.y = y;
    }

    vec2 getPos()
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
    void setRealDim(int rw, int rh)
    {
        real_rect.w = rw;
        real_rect.h = rh;
    }
    void setRealDim(vec2 rdim)
    {
        real_rect.w = rdim.x;
        real_rect.h = rdim.y;
    }
    vec2 getDim()
    {
        return vec2(rect.w, rect.h);
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

    void nextFrame(int afc, int afps) // FrameCount, FPS
    {
        if(afps > 0)
        {
            updatePath();

            if(frameIter == TARGET_FPS/afps) // TARGET_FPS/frameRate MUST BE INT
            {
                frameIter=0;
                if(curFrame == afc-1)
                    curFrame=0;
                else
                    curFrame++;
            }
            else
                frameIter++;
        }
    }

    SDL_Rect &healthBar()
    {
        health_rect.x = rect.x;
        health_rect.y = rect.y-HEALTHBAR_HEIGHT;
        health_rect.w = rect.w;
        health_rect.h = HEALTHBAR_HEIGHT;
        return health_rect;
    }

    void setTexs(QStringList texs, QList<SDL_Rect> frames);

    void moveTo(vec2 target);
    void moveTo(int tx, int ty);
    void updatePath();
};

#endif // ACTOR2D_H
