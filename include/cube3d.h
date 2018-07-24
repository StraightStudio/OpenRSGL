#ifndef CUBE3D_H
#define CUBE3D_H

#include <depends.h>
#include <vec3.h>

class Cube3D
{
public:
    Cube3D();
    Cube3D(vec3 p);

    vec3 pos;
    vec3 size;

    bool collides(Cube3D other);

    bool empty()
    {
        if(size.X() == 0 || size.Y() == 0 || size.Z() == 0)
            return true;
        return false;
    }
};

#endif // CUBE3D_H
