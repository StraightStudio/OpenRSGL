#ifndef VEC2_H
#define VEC2_H

#include <depends.h>

class vec2
{
public:
    int x, y;
    vec2(int a, int b):
        x(a), y(b){}
    vec2();

    vec2 &operator+(vec2 a);
    vec2 &operator-(vec2 a);

    unistring toString();

    int X(){return x;}
    int Y(){return y;}
    int W(){return x;}
    int H(){return y;}

};

#endif // VEC2_H
