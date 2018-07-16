#ifndef ACTOR2D_H
#define ACTOR2D_H

#ifdef _WIN64
#include <depends.h>
#include <logger.h>
#include <vec2.h>
#else
#include <include/depends.h>
#include <include/logger.h>
#include <include/vec2.h>
#endif

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
    unistring curAnim;
    unistring texture;

    vector<unistring> animations; // idle, walk, attack, die, etc...
    unistring idle_anim;      // animation default name: soldier, tank, etc. ( will be soldier_up, soldier_down )

    int curFrame; int frameIter;
    unistring name;
    unistring type;
    map<unistring,unistring> trigger;

    // ========== G A M E   L O G I C   V A R I A B L E S ===========================
    unistring parent; // Player or PC or another player.
    bool visible;

    vec2 targetPos; // Moving to...
    int move_speed;
    int move_direction;
    //
    int health_percentage;
    SDL_Rect health_rect;
    unistrlist sel_taunts;
    unistrlist mov_taunts;

    // Building variables
    vec2 so; // Spawn Offset
    bool structSelected;
    unistring structType;
    unistrlist punits; // Producing units.
    // ==============================================================================

    unistring tex()
    {
        return texture;
    }

    void setTex(unistring t)
    {
        texture = t;
    }

    void setSO(vec2 s)
    {
        so = s;
    }

    void setSO(int sx, int sy)
    {
        so.x = sx;
        so.y = sy;
    }

    void setStructType(unistring t)
    {
        structType = t;
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

    void setName(unistring n)
    {
        name = n;
    }

    unistring getName()
    {
        return name;
    }

    SDL_Rect &getRect()
    {
        return rect;
    }

    int triggerAction(unistring event)
    {
        if(trigger.count(event) > 0)
        {
            unistrlist parts;
            parts.resize(2, "");
            split(parts, trigger.at(event), is_any_of(" "), token_compress_on);

            if(parts[0] == "snd")
                return SOUND_ACTION;
            else if(parts[0] == "scn")
                return SCENE_ACTION;
            else if(parts[0] == "spw")
                return SPW_ACTION;
            else if(parts[0] == "quit")
                return QUIT_ACTION;
        }
        return -1;
    }
    unistring triggerArgument(unistring event)
    {
        if(trigger.count(event) > 0)
        {
            unistrlist parts;
            parts.resize(2, "");
            split(parts, trigger.at(event), is_any_of(" "), token_compress_on);

            if(parts.size() > 1)
                return parts[1];
            else
                return "";
        }
        return "";
    }

    void setType(unistring t)
    {
        type = t;
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

    unistring taunt(unistring t)
    {
        srand(time(nullptr));
        int rt=0;
        transform(t.begin(), t.end(), t.begin(), (int (*)(int))toupper);
        if(t == "SELECT")
        {
            rt = random() % sel_taunts.size();
            return sel_taunts[rt];
        }
        else if(t == "MOVE")
        {
            rt = random() % mov_taunts.size();
            return mov_taunts[rt];
        }
        return "";
    }

    SDL_Rect &healthBar()
    {
        health_rect.x = rect.x;
        health_rect.y = rect.y-HEALTHBAR_HEIGHT;
        health_rect.w = rect.w;
        health_rect.h = HEALTHBAR_HEIGHT;
        return health_rect;
    }

    void reset()
    {
        name.clear();
        type.clear();


        frameIter   = 0;
        curFrame    = 0;
        idle_anim   = "idle";
        curAnim     = idle_anim;
        texture.clear();


        rect.x      = 0; rect.y = 0;
        rect.w      = 0; rect.h = 0;

        real_rect.x = 0; real_rect.y = 0;
        real_rect.w = 0; real_rect.h = 0;


        sel_taunts.clear();
        mov_taunts.clear();


        move_direction = IDLE;
        move_speed  = 1;
        visible     = true;


        so.x = 0; so.y = 0;
        structType.clear();
        punits.clear();

        animations.clear();
        trigger.clear();
    }

    void moveTo(vec2 target);
    void moveTo(int tx, int ty);
    void updatePath();
};

#endif // ACTOR2D_H
