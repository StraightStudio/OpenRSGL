#ifndef LOADER3D_H
#define LOADER3D_H
#include <depends.h>
#include <vec3.h>

class Loader3D
{
public:
	Loader3D();
	void LoadModel(unistring fname);
	vector <GLfloat> vertices;	
	void* GetModel(); 
	GLfloat tmp[256];
};

#endif