#include <include/vec2.h>

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

QString vec2::toString()
{
    return "("+QString::number(x)+","+QString::number(y)+")";
}
