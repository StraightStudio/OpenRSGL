#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#ifdef _WIN64
#include <depends.h>
#include <config.h>
#include <actor2d.h>
#else
#include <include/depends.h>
#include <include/config.h>
#include <include/actor2d.h>
#endif

struct Action {
    int id;
    unistring data;
    vec2 transform_data[2];

    Action() :
        id(0), data("")
    {
        transform_data[0] = vec2(0, 0);
        transform_data[1] = vec2(0, 0);
    }
    Action(int i, unistring d) : id(i), data(d)
    {
        transform_data[0] = vec2(0, 0);
        transform_data[1] = vec2(0, 0);
    }

    Action(int i, unistring d, vec2 da, vec2 dd)
        : id(i), data(d)
    {
        transform_data[0] = da;
        transform_data[1] = dd;
    }

    ~Action()
    {
        data.clear();
        id=0;
        transform_data[0]=vec2(0,0);
        transform_data[1]=vec2(0,0);
    }

    bool boolData()
    {
        if(data == "true" || data == "TRUE")
            return true;
        return false;
    }

    void reset()
    {
        id = 0;
        data.clear();
    }

    vec2 vec2Data(int i)
    {
        return transform_data[i];
    }
};

class GameEvents
{
public:
    GameEvents();
    vec2 oldmpos;

    vec2 mousePos();
    bool rectOverlap(SDL_Rect &rect_1, SDL_Rect &rect_2);
    bool isMouseOver(SDL_Rect &rect);
    bool isMouseDown(int mbtn);
    bool isMouseUp(int mbtn);
    bool isMouseClicked(int mbtn);
    bool keyDown(int scancode);
    bool isSelected(Actor2d &a);

    Action processUIobject(Actor2d &obj);
    Action processActor(Actor2d &obj);
    Action processBuilding(Actor2d &obj);
    void addSelected(const map<unistring, Actor2d> &objs, SDL_Rect &selrect);

    map<unistring, Actor2d> selectionList;

    unistring mouse_state;
    bool button_down[3];
    bool button_clicked[3];
private:
    map<unistring, unistring> ui_btns;
    SDL_Event e;
};

#endif // GAMEEVENTS_H
