#include "../include/gameevents.h"

vec2 GameEvents::mousePos()
{
    int x,y;
    SDL_GetMouseState(&x, &y);
    return vec2(x, y);
}

bool GameEvents::isMouseOver(SDL_Rect *rect)
{
    int x,y;
    SDL_GetMouseState(&x, &y);
    if(x <= rect->x+rect->w && x >= rect->x &&
       y <= rect->y+rect->h && y >= rect->y)
        return true;
    else
        return false;
}

bool GameEvents::isMouseDown(int mbtn)
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mbtn); // 1 - left, 2 - middle, 3 - right
}

bool GameEvents::isMouseUp(int mbtn)
{
    return !( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mbtn) ); // 1 - left, 2 - middle, 3 - right
}

Action GameEvents::processUIobject(Actor2d &obj)
{
    vec2 res; /* res - resizing */ vec2 trs; /* trs - translation */
    if(isMouseOver(&obj.getRect()))
    {
        if(ui_btns[obj.getName()] != "hovered")
        {
            res = vec2(obj.getRect().w+16, obj.getRect().h+16);
            trs = vec2(obj.getRect().x-8, obj.getRect().y-8);
            ui_btns[obj.getName()] = "hovered";
            switch(obj.triggerAction("hover"))
            {
                case SOUND_ACTION:
                    return Action(POS_RES_SND_ACTION, obj.triggerArgument("hover"), res, trs);
                break;
            }
            return Action(POS_RES_ACTION, 0, res, trs);
        }
        if(isMouseDown(1))
        {
            switch(obj.triggerAction("click"))
            {
                case SCENE_ACTION:
                    return Action(SCENE_ACTION, obj.triggerArgument("click"));
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
            res = vec2(obj.getRect().w-16, obj.getRect().h-16);
            trs = vec2(obj.getRect().x+8, obj.getRect().y+8);
            ui_btns[obj.getName()] = "none";
            return Action(POS_RES_ACTION, 0, res, trs);
        }
    }
    return Action(0, "0");
}

Action GameEvents::processActor(Actor2d &obj)
{
    vec2 res; /* res - resizing */ vec2 trs; /* trs - translation */
    if(isMouseOver(&obj.getRect()))
    {
        if(isMouseDown(1))
        {
            selectionList[obj.getName()] = obj;
        }
    }
    else
    {
        if(isMouseDown(1))
        {
            selectionList.remove(obj.getName());
        }
        if(isMouseDown(3))
        {
            if(selectionList.contains( obj.getName() ) )
            {
                trs = vec2( mousePos().x-obj.rect.w/2, mousePos().y-obj.rect.h+16 );
                return Action(MOV_ACTION, 0, res, trs);
            }
        }
    }
    return Action(0, "0");
}

GameEvents::GameEvents()
{

}
