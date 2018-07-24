#include <vec3.h>

vec3::vec3()
{

}

vec3 &vec3::operator+(vec3 a)
{
    x = a.x+x;
    y = a.y+y;
    z = a.z+z;
    return *this;
}

vec3 &vec3::operator-(vec3 a)
{
    x = x-a.x;
    y = y-a.y;
    z = z-a.z;
    return *this;
}

unistring vec3::toString()
{
    return "("+to_string(x)+";"+to_string(y)+";"+to_string(z)+")";
}
