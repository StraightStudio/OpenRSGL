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
    // Get/Set id, view_model, obj_model functions, (void type for Set, unistring/int_fast16_t for Get)

private:
    unistring name;
    int_fast16_t id;

    unistring view_model;
    unistring obj_model;
};

#endif // RESOURCE_H
