#include <cube3d.h>

Cube3D::Cube3D() :
    pos(0,0,0),
    size(0,0,0)
{




}

Cube3D::Cube3D(glm::vec3 p) :
    pos(p),
    size(0,0,0)
{

}

Cube3D::Cube3D(glm::vec2 p) :
    pos(glm::vec3(p.x, p.y, 0)),
    size(0,0,0)
{

}

bool Cube3D::collides(Cube3D other)
{
    return false;
}

bool Cube3D::consist(glm::vec2 point)
{
    if(point.x >= pos.x-size.x/2 && point.x <= pos.x+size.x/2)
    {
        return true;
    }
    return false;
}
