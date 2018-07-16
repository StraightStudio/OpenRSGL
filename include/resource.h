#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef _WIN64
#include <depends.h>
#else
#include <include/depends.h>
#endif

class Resource
{
public:
    Resource();
private:
    unistring name;
    int_fast16_t id;

    unistring view_model;
    unistring obj_model;
};

#endif // RESOURCE_H
