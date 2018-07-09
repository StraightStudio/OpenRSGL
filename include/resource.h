#ifndef RESOURCE_H
#define RESOURCE_H

#include <include/depends.h>

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
