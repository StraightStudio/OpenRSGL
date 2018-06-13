#include "../include/gameevents.h"

GameEvents::GameEvents()
{
    mouse_state = "none";
    button_down[0] = false;
    button_down[1] = false;
    button_down[2] = false;

    button_clicked[0] = false;
    button_clicked[1] = false;
    button_clicked[2] = false;
}

vec2 GameEvents::mousePos()
{
    int x,y;
    SDL_GetMouseState(&x, &y);
    return vec2(x, y);
}

bool GameEvents::rectOverlap(SDL_Rect &rect_1, SDL_Rect &rect_2)
{
    SDL_Rect res;
    if(SDL_IntersectRect(&rect_1, &rect_2, &res) == SDL_TRUE)
        return true;
    else
        return false;
}

bool GameEvents::isMouseOver(SDL_Rect &rect)
{
    SDL_Rect mr;
    mr.w = 16;
    mr.h = 16;
    int x,y;
    SDL_GetMouseState(&x, &y);
    mr.x = x;
    mr.y = y;

    if(rectOverlap(mr, rect))
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
    return SDL_GetKeyboardState(NULL)[scancode];
}

bool GameEvents::isSelected(Actor2d &a)
{
    if(selectionList.contains(a.getName()))
        return true;
    return false;
}

void GameEvents::addSelected(const QList<Actor2d> &objs, SDL_Rect &selrect)
{
    for(Actor2d a : objs)
    {
        if(rectOverlap(a.real_rect, selrect) && a.type == "actor")
        {
            if(!selectionList.contains(a.getName()))
                selectionList[a.getName()] = a;
        }
    }
}

Action GameEvents::processUIobject(Actor2d &obj)
{
    vec2 res; /* res - resizing */ vec2 trs; /* trs - translation */
    if(!obj.visible)
        return Action(0, "0");
    if(isMouseOver(obj.getRect()))
    {
        if(ui_btns[obj.getName()] != "hovered")
        {
            res = vec2(obj.getRect().w+2, obj.getRect().h+2);
            trs = vec2(obj.getRect().x-1, obj.getRect().y-1);
            ui_btns[obj.getName()] = "hovered";
            switch(obj.triggerAction("hover"))
            {
                case SOUND_ACTION:
                    return Action(POS_RES_SND_ACTION, obj.triggerArgument("hover"), res, trs);
                break;
            }
            return Action(POS_RES_ACTION, 0, res, trs);
        }
        if(isMouseClicked(SDL_BUTTON_LEFT))
        {
            switch(obj.triggerAction("click"))
            {
                case SCENE_ACTION:
                    return Action(SCENE_ACTION, obj.triggerArgument("click"));
                break;
                case SPW_ACTION:
                    return Action(SPW_ACTION, obj.triggerArgument("click"));
                break;
                case QUIT_ACTION:
                    return Action(QUIT_ACTION, true);
                break;
            }
        }
    }
    else
    {
        if(ui_btns[obj.getName()] == "hovered")
        {
            res = vec2(obj.getRect().w-2, obj.getRect().h-2);
            trs = vec2(obj.getRect().x+1, obj.getRect().y+1);
            ui_btns[obj.getName()] = "none";
            return Action(POS_RES_ACTION, 0, res, trs);
        }
    }
    return Action(0, "0");
}

Action GameEvents::processActor(Actor2d &obj)
{
    vec2 res; /* res - resizing */ vec2 trs; /* trs - translation */
    if(!obj.visible)
        return Action(0, "0");

    if(isMouseOver(obj.rect))
    {
        if(mouse_state != "over")
            mouse_state = "over";

        if(isMouseClicked(SDL_BUTTON_LEFT))
        {
            Logger::log("GameEvents", "Selected '"+obj.getName()+"'");
            selectionList.insert(obj.getName(), obj);
            return Action(SOUND_ACTION, obj.taunt("SELECT"));
        }
    }
    else
    {
        if(isMouseClicked(SDL_BUTTON_LEFT))
        {
            if(!keyDown(SDL_SCANCODE_LSHIFT))
                selectionList.remove(obj.getName());
        }
        if(isMouseClicked(SDL_BUTTON_RIGHT))
        {
            if( selectionList.contains(obj.getName()) )
            {
                trs = vec2( mousePos().x-obj.rect.w/2, mousePos().y-obj.rect.h+16 );
                return Action(SND_MOV_ACTION, obj.taunt("MOVE"), res, trs);
            }
        }
    }
    return Action(0, "0");
}

Action GameEvents::processBuilding(Actor2d &obj)
{
    vec2 res; /* res - resizing */ vec2 trs; /* trs - translation */
    if(!obj.visible)
        return Action(0, "0");
    if(isMouseOver(obj.real_rect))
    {
        if(mouse_state != "over")
            mouse_state = "over";
        if(isMouseClicked(SDL_BUTTON_LEFT))
        {
            Logger::log("GameEvents", "Selected "+obj.getName());
            return Action(SELECTED_BUILDING_ACTION, obj.getName()+":"+obj.structType);
        }
    }
    else
    {
        if(isMouseClicked(SDL_BUTTON_LEFT))
        {
            Logger::log("GameEvents", "DESelected "+obj.getName());
            return Action(DESELECTED_BUILDING_ACTION, obj.getName()+":"+obj.structType);
        }
    }
}
