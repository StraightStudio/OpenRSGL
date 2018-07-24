#ifndef VEC2_H
#define VEC2_H

#include <depends.h>

class vec3
{
public:
    float x, y, z;
    vec3(float a, float b, float c):
        x(a), y(b), z(c){}
    vec3();

    vec3 &operator+(vec3 a);
    vec3 &operator-(vec3 a);

    unistring toString();

    float X(){return x;}
    float Y(){return y;}
    float Z(){return z;}
    float W(){return x;}
    float H(){return y;}
    float D(){return z;}
};

#endif // VEC2_H
