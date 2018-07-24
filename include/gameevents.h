#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#include <depends.h>
#include <config.h>
#include <actor2d.h>
#include <cube3d.h>

struct Action {
    int id;
    unistring data;
    vec3 transform_data[2];

    Action() :
        id(0), data("")
    {
        transform_data[0] = vec3(0, 0, 0);
        transform_data[1] = vec3(0, 0, 0);
    }
    Action(int i, unistring d) : id(i), data(d)
    {
        transform_data[0] = vec3(0, 0, 0);
        transform_data[1] = vec3(0, 0, 0);
    }

    Action(int i, unistring d, vec3 da, vec3 dd)
        : id(i), data(d)
    {
        transform_data[0] = da;
        transform_data[1] = dd;
    }

    ~Action()
    {
        data.clear();
        id=0;
        transform_data[0]=vec3(0, 0, 0);
        transform_data[1]=vec3(0, 0, 0);
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

    vec3 vec3Data(int i)
    {
        return transform_data[i];
    }
};

class GameEvents
{
public:
    GameEvents();
    vec3 oldmpos;

    vec3 mousePos();
    bool rectOverlap(Cube3D &first, Cube3D &last);
    bool isMouseOver(Cube3D &other);
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
