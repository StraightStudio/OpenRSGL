#ifndef LOADER3D_H
#define LOADER3D_H
#include <depends.h>
#include <vec3.h>
#include <logger.h>

class Loader3D
{
public:
	Loader3D();
    ~Loader3D();
    void LoadModel(unistring fname, unistring alias);
	vector <GLfloat> vertices;	
    GLfloat *GetModel(unistring mdl);
    size_t GetSize(unistring mdl);

    void cacheModel(unistring mdl);

    map<unistring, vector<GLfloat>> model_map;
    GLfloat* cache;
    unistring cachedModel;
};

#endif
