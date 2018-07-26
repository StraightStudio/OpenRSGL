#ifndef CUBE3D_H
#define CUBE3D_H

#include <depends.h>

class Cube3D
{
public:
    Cube3D();
    Cube3D(glm::vec3 p);
    Cube3D(glm::vec2 p);

    glm::vec3 pos;
    glm::vec3 size;

    bool collides(Cube3D other);

    bool empty()
    {
        if(size.x == 0 || size.x == 0 || size.x == 0)
            return true;
        return false;
    }
};

#endif // CUBE3D_H
