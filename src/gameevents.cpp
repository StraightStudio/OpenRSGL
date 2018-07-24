#include <gameevents.h>

GameEvents::GameEvents()
{
    mouse_state = "none";
    oldmpos.x = 0;  oldmpos.y = 0;

    button_down[0] = false;
    button_down[1] = false;
    button_down[2] = false;

    button_clicked[0] = false;
    button_clicked[1] = false;
    button_clicked[2] = false;
}

vec3 GameEvents::mousePos()
{
    int x,y;
    SDL_GetMouseState(&x, &y);
    return vec3(x, y, 0);
}

bool GameEvents::rectOverlap(Cube3D &first, Cube3D &last)
{
    if(first.empty() || last.empty())
        return false;

    if(first.collides(last))
        return true;
    else
        return false;
}

bool GameEvents::isMouseOver(Cube3D &other)
{
    if(other.collides( Cube3D(mousePos()) ))
        return true;
    else
        return false;
}

bool GameEvents::isMouseDown(int mbtn)
{
    return button_down[mbtn-1]; // 1 - left, 2 - middle, 3 - right
}

bool GameEvents::isMouseUp(int mbtn)
{
    return !button_down[mbtn-1];
}

bool GameEvents::isMouseClicked(int mbtn)
{
    return button_clicked[mbtn-1];
}

bool GameEvents::keyDown(int scancode)
{
    return SDL_GetKeyboardState(nullptr)[scancode];
}

bool GameEvents::isSelected(Actor2d &a)
{
    if(selectionList.count(a.getName()) > 0)
        return true;
    return false;
}

void GameEvents::addSelected(const map<unistring, Actor2d> &objs, SDL_Rect &selrect)
{

}

Action GameEvents::processUIobject(Actor2d &obj)
{
    return Action(0, "");
}

Action GameEvents::processActor(Actor2d &obj)
{
    return Action(0, "");
}

Action GameEvents::processBuilding(Actor2d &obj)
{
    return Action(0, "");
}
