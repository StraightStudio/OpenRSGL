#include <actor2d.h>

Actor2d::Actor2d()
{
    reset();
}

void Actor2d::moveTo(vec2 target)
{
    frameIter=0;
    curFrame=0;

    targetPos = target;
}

void Actor2d::moveTo(int tx, int ty)
{
    moveTo(vec2(tx, ty));
}

void Actor2d::updatePath()
{
    if( rect.x < targetPos.x && rect.y == targetPos.y )
    {
        // LEFT
        if(move_direction != RIGHT_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_right";
            move_direction = RIGHT_DIR;
        }
        rect.x += move_speed;
    }
    else if( rect.x > targetPos.x && rect.y == targetPos.y )
    {
        // RIGHT
        if(move_direction != LEFT_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_left";
            move_direction = LEFT_DIR;
        }
        rect.x -= move_speed;
    }
    else if( rect.y < targetPos.y && rect.x == targetPos.x )
    {
        // DOWN
        if(move_direction != DOWN_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_down";
            move_direction = DOWN_DIR;
        }
        rect.y += move_speed;
    }
    else if( rect.y > targetPos.y && rect.x == targetPos.x)
    {
        // UP
        if(move_direction != UP_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_up";
            move_direction = UP_DIR;
        }
        rect.y -= move_speed;
    }

    else if( rect.x < targetPos.x && rect.y < targetPos.y )
    {
        // DOWN+LEFT
        if(move_direction != DOWN_RIGHT_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_down_right";
            move_direction = DOWN_RIGHT_DIR;
        }
        rect.x += move_speed;
        rect.y += move_speed;
    }
    else if( rect.x < targetPos.x && rect.y > targetPos.y )
    {
        // UP+LEFT
        if(move_direction != UP_RIGHT_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_up_right";
            move_direction = UP_RIGHT_DIR;
        }
        rect.x += move_speed;
        rect.y -= move_speed;
    }
    else if( rect.x > targetPos.x && rect.y > targetPos.y )
    {
        // UP+RIGHT
        if(move_direction != UP_LEFT_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_up_left";
            move_direction = UP_LEFT_DIR;
        }
        rect.x -= move_speed;
        rect.y -= move_speed;
    }
    else if( rect.x > targetPos.x && rect.y < targetPos.y )
    {
        // DOWN+RIGHT
        if(move_direction != DOWN_LEFT_DIR)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim+"_down_left";
            move_direction = DOWN_LEFT_DIR;
        }
        rect.x -= move_speed;
        rect.y += move_speed;
    }

    else
    {
        if(move_direction != IDLE)
        {
            frameIter = 0;
            curFrame = 0;
            curAnim = idle_anim;
            move_direction = IDLE;
        }
    }
    real_rect.x = rect.x+(rect.w-real_rect.w)/2;
    real_rect.y = rect.y+(rect.h-real_rect.h)/2;
}
