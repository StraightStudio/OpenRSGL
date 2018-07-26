#ifndef LOADER3D_H
#define LOADER3D_H

#include <depends.h>
#include <object3d.h>
#include <vec3.h>
#include <logger.h>

class Loader3D
{
public:
	Loader3D();
    ~Loader3D();

    void LoadModel(unistring fname, Object3d &target);
};

#endif
