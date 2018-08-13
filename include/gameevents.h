#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#include <depends.h>
#include <config.h>
#include <camera.h>
#include <cube3d.h>

class Core;
#include <core.h>

class GameEvents
{
public:
    GameEvents();
    glm::vec2 oldmpos;

    glm::vec2 mousePos();
    bool rectOverlap(Cube3D &first, Cube3D &last);
    bool isMouseOver(Camera* m_cam, Cube3D* other);
    bool isMouseDown(int mbtn);
    bool isMouseUp(int mbtn);
    bool isMouseClicked(int mbtn);
    bool keyDown(int scancode);

    bool isParallel(glm::vec2 first, glm::vec2 second);

    unistring mouse_state;
    bool button_down[3];
    bool button_clicked[3];
};

#endif // GAMEEVENTS_H
