#include <vec2.h>

vec2::vec2()
{

}

vec2 &vec2::operator+(vec2 a)
{
    x = a.x+x;
    y = a.y+y;
    return *this;
}

vec2 &vec2::operator-(vec2 a)
{
    x = x-a.x;
    y = y-a.y;
    return *this;
}

unistring vec2::toString()
{
    return "("+to_string(x)+","+to_string(y)+")";
}
